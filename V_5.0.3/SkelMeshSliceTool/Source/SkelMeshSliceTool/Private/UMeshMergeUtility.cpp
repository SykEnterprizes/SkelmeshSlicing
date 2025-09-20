// Fill out your copyright notice in the Description page of Project Settings.

#include "UMeshMergeUtility.h"
// includes for the skeletalmesh codes
#include <Rendering/SkeletalMeshLODModel.h>

#include "Rendering/SkeletalMeshModel.h"

#include "Engine/SkeletalMesh.h"
//#include "Rendering/SkeletalMeshRenderData.h"
//#include "Rendering/SkeletalMeshLODRenderData.h"

// includes for the static mesh codes

#include "Engine/StaticMesh.h"
#include "StaticMeshAttributes.h"
#include "MeshDescription.h"
#include "MeshDescriptionBuilder.h"
//#include "StaticMeshOperations.h"
#include "AssetRegistryModule.h"
// include for the Modal task helper
#include "UModalTaskHelper.h"

#if WITH_EDITOR
#include "Editor.h"                          // for GEditor
#include "Subsystems/AssetEditorSubsystem.h" // for UAssetEditorSubsystem
#endif

void UUMeshMergeUtility::ConvertProceduralMeshToRawMeshData(
	TArray<UProceduralMeshComponent*> ProcMesh,
	FRawMeshData& OutRaw)
 {
	int32 VertexOffset = OutRaw.Vertices.Num();

	for (int32 MeshIndex = 0; MeshIndex < ProcMesh.Num(); ++MeshIndex)
	{
		const FProcMeshSection* Section = ProcMesh[MeshIndex]->GetProcMeshSection(0);
		if (!Section) continue;

		int32 RemapCounter = 0;

		// Add vertices
		for (int32 VertIndex = 0; VertIndex < Section->ProcVertexBuffer.Num(); ++VertIndex)
		{
			const FProcMeshVertex& V = Section->ProcVertexBuffer[VertIndex];

			FRawVertexData Raw;
			Raw.Position = FVector3f(V.Position);
			Raw.OriginalVertexIndex = VertIndex;
			Raw.RemappedIndex = RemapCounter++;
			Raw.CompactBoneIndex = 0;
			Raw.BoneInfluences.Empty();
			Raw.BoneWeights.Empty();
			Raw.BoneNames.Empty();

			OutRaw.Vertices.Add(Raw);
			OutRaw.UVs.Add(V.UV0);
			OutRaw.Normals.Add(V.Normal);
			OutRaw.Tangents.Add(V.Tangent.TangentX);
			OutRaw.Colors.Add(V.Color);
		}

		// Add triangles with offset
		for (int32 TriIndex = 0; TriIndex < Section->ProcIndexBuffer.Num(); TriIndex += 3)
		{
			OutRaw.Triangles.Add(FIntVector(
				Section->ProcIndexBuffer[TriIndex] + VertexOffset,
				Section->ProcIndexBuffer[TriIndex + 1] + VertexOffset,
				Section->ProcIndexBuffer[TriIndex + 2] + VertexOffset
			));
		}

		VertexOffset = OutRaw.Vertices.Num(); // Update for next mesh
	}
}

void UUMeshMergeUtility::ExtractRawMeshDataFromSkeletalMesh(USkeletalMesh* SkeletalMesh, FRawMeshData& OutRaw)
{
	// make sure theres a valid skeletal mesh and it has LODs
	if (!SkeletalMesh || !SkeletalMesh->GetImportedModel() || SkeletalMesh->GetImportedModel()->LODModels.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("? Invalid skeletal mesh or missing LODs"));
		return;
	}
	// access the first LOD model so we can write into it
	const FSkeletalMeshLODModel& LODModel = SkeletalMesh->GetImportedModel()->LODModels[0];

	int32 RemapCounter = 0;

	for (const FSkelMeshSection& Section : LODModel.Sections)
	{
		for (int32 VertexIdx = 0; VertexIdx < Section.SoftVertices.Num(); ++VertexIdx)
		{
			const FSoftSkinVertex& V = Section.SoftVertices[VertexIdx];

			FRawVertexData Raw;
			Raw.Position = V.Position;
			Raw.OriginalVertexIndex = VertexIdx;
			Raw.CompactBoneIndex = Section.BoneMap[V.InfluenceBones[0]]; // Primary bone
			Raw.RemappedIndex = RemapCounter++;
		
			for (int32 i = 0; i < MAX_TOTAL_INFLUENCES; ++i)
			{
				if (V.InfluenceWeights[i] > 0)
				{
					int32 BoneIndex = Section.BoneMap[V.InfluenceBones[i]];
					Raw.BoneInfluences.Add(BoneIndex);
					Raw.BoneWeights.Add(V.InfluenceWeights[i] / 255.0f);

					// Optional: resolve bone name
					if (SkeletalMesh->GetRefSkeleton().IsValidIndex(BoneIndex))
					{
						FName BoneName = SkeletalMesh->GetRefSkeleton().GetBoneName(BoneIndex);
						Raw.BoneNames.Add(BoneName);
					}
				}
			}

			OutRaw.Vertices.Add(Raw);
			OutRaw.UVs.Add(FVector2d(V.UVs[0]));
			OutRaw.Normals.Add(FVector(V.TangentZ));
			OutRaw.Tangents.Add(FVector(V.TangentX));
			OutRaw.Colors.Add(V.Color);
		}
	}

	// Extract triangles WITH material assignments
	int32 TriangleIndex = 0;
	for (const FSkelMeshSection& Section : LODModel.Sections)
	{
		int32 MaterialIndex = Section.MaterialIndex; // GET THE MATERIAL INDEX!

		int32 num = Section.NumTriangles;
		// Process triangles for this section
		for (int32 i = 0; i < num; ++i)
		{
			int32 IndexBase = Section.BaseIndex + (i * 3);

			if (IndexBase + 2 < LODModel.IndexBuffer.Num())
			{
				OutRaw.Triangles.Add(FIntVector(
					LODModel.IndexBuffer[IndexBase],
					LODModel.IndexBuffer[IndexBase + 1],
					LODModel.IndexBuffer[IndexBase + 2]
				));

				// CRITICAL: Store the material index for this triangle
				OutRaw.TriangleMaterialIndices.Add(MaterialIndex);
				TriangleIndex++;
			}
		}
	}

	// Optional: extract UVs, normals, tangents separately if needed
	/*for (const FRawVertexData& V : OutRaw.Vertices)
	{
		OutRaw.UVs.Add(V.UV);
		OutRaw.Normals.Add(V.Normal);
		OutRaw.Tangents.Add(V.Tangent);
		OutRaw.Colors.Add(V.Color);
	}*/
}

void UUMeshMergeUtility::ExtractRawMeshDataFromCachedMeshContext(FMeshBuildContext Context, FRawMeshData& OutRaw)
{
	// make sure theres a valid skeletal mesh and it has LODs
	if (Context.FinalTriangles.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("? Invalid static mesh or missing LODs"));
		return;
	}

	for (FVector3f vec : Context.FinalVertices)
	{
		FRawVertexData Raw;  // Raw vertex data is an array of vertices
		Raw.Position = vec;
		OutRaw.Vertices.Add(Raw);
		OutRaw.Colors.Add(FColor::White); // Default color, can be changed later
	}

	OutRaw.UVs = Context.FinalUVs;
	OutRaw.Normals = Context.FinalNormals;
	OutRaw.Tangents = Context.FinalTangents;
	OutRaw.Triangles = Context.IntVecTriangles;

	// FIX: Copy the material indices from context
	OutRaw.TriangleMaterialIndices = Context.FinalMaterialIndices;

	// Handle potential size mismatch between triangles and material indices
	if (OutRaw.TriangleMaterialIndices.Num() > OutRaw.Triangles.Num())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Material indices array is larger than triangles array - trimming to match"));
		OutRaw.TriangleMaterialIndices.SetNum(OutRaw.Triangles.Num());
	}
	else if (OutRaw.TriangleMaterialIndices.Num() < OutRaw.Triangles.Num())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Material indices array is smaller than triangles array - padding with material 0"));
		while (OutRaw.TriangleMaterialIndices.Num() < OutRaw.Triangles.Num())
		{
			OutRaw.TriangleMaterialIndices.Add(0);
		}
	}

	
}

// Fixed version that properly aligns the data structures
static void AutoSkinSection2(
	const FReferenceSkeleton& RefSkeleton,
	const TArray<FRawVertexData>& RawVertexData,  // ONLY authored (base mesh) vertices
	const TArray<FVector>& AllVertices,           // ALL vertices (base + caps)
	const TArray<FVector>& AllNormals,            // ALL normals (base + caps)  
	const TArray<FVector>& AllTangents,           // ALL tangents (base + caps)
	const TArray<FVector2D>& AllUVs,              // ALL UVs (base + caps)
	const TArray<FColor>& AllColors,              // ALL colors (base + caps)
	FSkeletalMeshLODModel& LODModel,
	FSkelMeshSection& Section,
	TMap<int32, FCapSkinningInfo>& CapSkinningInfos,
	float ProximityRadius = 10.f,
	int32 MaxInfluences = MAX_TOTAL_INFLUENCES)
{
	
	// Validate data alignment
	if (RawVertexData.Num() > AllVertices.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Data mismatch: RawVertexData (%d) > AllVertices (%d)"),
			RawVertexData.Num(), AllVertices.Num());
		return;
	}

	// 1) Collect bone names from authored data
	TSet<FName> BoneNamesInUse;
	for (const FRawVertexData& Raw : RawVertexData)
	{
		for (const FName& BN : Raw.BoneNames)
		{
			if (BN != NAME_None)
			{
				BoneNamesInUse.Add(BN);
			}
		}
	}

	// 2) Map bone names to skeleton indices
	TSet<uint16> BonesInUse;
	TMap<FName, int32> BoneNameToIndex;
	for (const FName& BN : BoneNamesInUse)
	{
		int32 Idx = RefSkeleton.FindBoneIndex(BN);
		if (Idx != INDEX_NONE)
		{
			BonesInUse.Add(Idx);
			BoneNameToIndex.Add(BN, Idx);
		}
	}

	// 3) Build BoneMap and register required bones
	Section.BoneMap = BonesInUse.Array();
	Section.BoneMap.Sort();

	for (int32 BoneIdx : Section.BoneMap)
	{
		LODModel.RequiredBones.AddUnique(BoneIdx);
		LODModel.ActiveBoneIndices.AddUnique(BoneIdx);
	}

	// 4) Build compact index map
	TMap<int32, int32> BoneToCompact;
	for (int32 i = 0; i < Section.BoneMap.Num(); ++i)
	{
		BoneToCompact.Add(Section.BoneMap[i], i);
	}

	// 5) PRE-CALCULATE BONE WORLD POSITIONS
	TArray<FVector> BoneWorldPositions;
	BoneWorldPositions.SetNum(RefSkeleton.GetNum());

	TArray<FTransform> ComponentSpaceTransforms;
	ComponentSpaceTransforms.SetNum(RefSkeleton.GetNum());

	for (int32 BoneIndex = 0; BoneIndex < RefSkeleton.GetNum(); ++BoneIndex)
	{
		FTransform BoneTransform = RefSkeleton.GetRefBonePose()[BoneIndex];
		int32 ParentIndex = RefSkeleton.GetParentIndex(BoneIndex);

		if (ParentIndex != INDEX_NONE)
		{
			ComponentSpaceTransforms[BoneIndex] = BoneTransform * ComponentSpaceTransforms[ParentIndex];
		}
		else
		{
			ComponentSpaceTransforms[BoneIndex] = BoneTransform;
		}

		BoneWorldPositions[BoneIndex] = ComponentSpaceTransforms[BoneIndex].GetLocation();
	}

	// 6) CRITICAL FIX: Allocate vertices for ALL vertices (not just authored ones)
	const int32 TotalVerts = AllVertices.Num();
	const int32 AuthoredVerts = RawVertexData.Num();
	Section.SoftVertices.SetNumZeroed(TotalVerts);  // This must match AllVertices.Num()

	UE_LOG(LogTemp, Warning, TEXT("📊 Allocated %d SoftVertices for %d total vertices"),
		Section.SoftVertices.Num(), TotalVerts);

	// 7) Initialize ALL vertices with basic data
	for (int32 i = 0; i < TotalVerts; ++i)
	{
		FSoftSkinVertex& V = Section.SoftVertices[i];
		V.Position = FVector3f(AllVertices[i]);
		V.TangentZ = FVector3f(AllNormals.IsValidIndex(i) ? AllNormals[i] : FVector::UpVector);
		V.TangentX = FVector3f(AllTangents.IsValidIndex(i) ? AllTangents[i] : FVector::ForwardVector);
		V.TangentY = FVector3f(FVector::CrossProduct(FVector(V.TangentZ), FVector(V.TangentX)));
		V.UVs[0] = FVector2f(AllUVs.IsValidIndex(i) ? AllUVs[i] : FVector2D::ZeroVector);
		V.Color = AllColors.IsValidIndex(i) ? AllColors[i] : FColor::White;

		// Initialize bone influences to zero
		for (int32 j = 0; j < MaxInfluences; ++j)
		{
			V.InfluenceBones[j] = 0;
			V.InfluenceWeights[j] = 0;
		}
	}

	// 8) Apply authored weights to base mesh vertices ONLY

	for (int32 i = 0; i < AuthoredVerts; ++i)
	{
		if (i >= Section.SoftVertices.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Index out of bounds: trying to access vertex %d, but only have %d"),
				i, Section.SoftVertices.Num());
			break;
		}

		const FRawVertexData& Raw = RawVertexData[i];
		FSoftSkinVertex& V = Section.SoftVertices[i];

		int32 Count = FMath::Min3(Raw.BoneNames.Num(), Raw.BoneWeights.Num(), MaxInfluences);
		int32 Valid = 0;
		float TotalWeight = 0.f;

		for (int32 j = 0; j < Count; ++j)
		{
			const FName& BoneName = Raw.BoneNames[j];
			float Weight = Raw.BoneWeights[j];
			if (Weight <= 0.f || BoneName == NAME_None) continue;

			const int32* RefIdx = BoneNameToIndex.Find(BoneName);
			const int32* CompactIdx = RefIdx ? BoneToCompact.Find(*RefIdx) : nullptr;
			if (!CompactIdx) continue;

			V.InfluenceBones[Valid] = static_cast<uint8>(*CompactIdx);
			V.InfluenceWeights[Valid] = FMath::Clamp(FMath::RoundToInt(Weight * 255.f), 1, 255);
			TotalWeight += V.InfluenceWeights[Valid++];
		}

		if (Valid == 0 || TotalWeight == 0.f)
		{
			V.InfluenceBones[0] = 0;
			V.InfluenceWeights[0] = 255;
		}
		else if (FMath::Abs(TotalWeight - 255.f) > 1.f)
		{
			float Scale = 255.f / TotalWeight;
			int32 Sum = 0;
			for (int32 j = 0; j < Valid; ++j)
			{
				V.InfluenceWeights[j] = FMath::Clamp(FMath::RoundToInt(V.InfluenceWeights[j] * Scale), 1, 255);
				Sum += V.InfluenceWeights[j];
			}
			if (Sum != 255)
			{
				V.InfluenceWeights[0] = FMath::Clamp(V.InfluenceWeights[0] + (255 - Sum), 1, 255);
			}
		}
	}

	// 9) Apply cap skinning to remaining vertices

	for (int32 i = AuthoredVerts; i < TotalVerts; ++i)
	{
		FSoftSkinVertex& V = Section.SoftVertices[i];

		// Find which cap this vertex belongs to
		ESkinningType SkinningMethodToUse = ESkinningType::RigidClosestBone;
		float RadiusToUse = 10.0f;
		int32 CapIndex = -1;

		for (const auto& CapPair : CapSkinningInfos)
		{
			const FCapSkinningInfo& CapInfo = CapPair.Value;
			if (i >= CapInfo.StartVertexIndex && i <= CapInfo.EndVertexIndex)
			{
				SkinningMethodToUse = CapInfo.SkinningMethod;
				RadiusToUse = CapInfo.ProximityRadius;
				CapIndex = CapPair.Key;
				break;
			}
		}

		if (SkinningMethodToUse == ESkinningType::RigidClosestBone)
		{
			// Find closest bone
			FVector VertexPos = AllVertices[i];
			float MinDistance = FLT_MAX;
			int32 ClosestBoneCompactIdx = 0;

			for (int32 BoneMapIdx = 0; BoneMapIdx < Section.BoneMap.Num(); ++BoneMapIdx)
			{
				int32 RefSkeletonIdx = Section.BoneMap[BoneMapIdx];
				FVector BonePos = BoneWorldPositions[RefSkeletonIdx];
				float Distance = FVector::Dist(VertexPos, BonePos);

				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					ClosestBoneCompactIdx = BoneMapIdx;
				}
			}

			V.InfluenceBones[0] = static_cast<uint8>(ClosestBoneCompactIdx);
			V.InfluenceWeights[0] = 255;

			if (CapIndex == 0) // Only log for first cap to avoid spam
			{
				int32 RefBoneIdx = Section.BoneMap[ClosestBoneCompactIdx];
				/*UE_LOG(LogTemp, Log, TEXT("Cap[%d] vertex %d: bound to %s (dist: %.2f)"),
					CapIndex, i, *RefSkeleton.GetBoneName(RefBoneIdx).ToString(), MinDistance);*/
			}
		}
		else if (SkinningMethodToUse == ESkinningType::ProximityBlending)
		{
			// Proximity weighted skinning implementation
			FVector VertexPos = AllVertices[i];
			TMap<int32, float> BoneWeights;
			float TotalInfluence = 0.0f;

			for (int32 BoneMapIdx = 0; BoneMapIdx < Section.BoneMap.Num(); ++BoneMapIdx)
			{
				int32 RefSkeletonIdx = Section.BoneMap[BoneMapIdx];
				FVector BonePos = BoneWorldPositions[RefSkeletonIdx];
				float Distance = FVector::Dist(VertexPos, BonePos);

				if (Distance <= RadiusToUse)
				{
					float Influence = FMath::Max(0.01f, 1.0f - (Distance / RadiusToUse));
					Influence = Influence * Influence; // Square for better falloff

					BoneWeights.Add(BoneMapIdx, Influence);
					TotalInfluence += Influence;
				}
			}

			if (BoneWeights.Num() == 0)
			{
				// Fallback to closest bone
				float MinDistance = FLT_MAX;
				int32 ClosestBoneCompactIdx = 0;

				for (int32 BoneMapIdx = 0; BoneMapIdx < Section.BoneMap.Num(); ++BoneMapIdx)
				{
					int32 RefSkeletonIdx = Section.BoneMap[BoneMapIdx];
					FVector BonePos = BoneWorldPositions[RefSkeletonIdx];
					float Distance = FVector::Dist(VertexPos, BonePos);

					if (Distance < MinDistance)
					{
						MinDistance = Distance;
						ClosestBoneCompactIdx = BoneMapIdx;
					}
				}

				V.InfluenceBones[0] = static_cast<uint8>(ClosestBoneCompactIdx);
				V.InfluenceWeights[0] = 255;
			}
			else
			{
				// Assign weighted influences
				TArray<TPair<int32, float>> SortedWeights;
				for (const auto& Pair : BoneWeights)
				{
					SortedWeights.Add(TPair<int32, float>(Pair.Key, Pair.Value));
				}
				SortedWeights.Sort([](const TPair<int32, float>& A, const TPair<int32, float>& B) {
					return A.Value > B.Value;
					});

				int32 NumInfluences = FMath::Min(SortedWeights.Num(), MaxInfluences);
				float WeightSum = 0.0f;

				for (int32 j = 0; j < NumInfluences; ++j)
				{
					float NormalizedWeight = SortedWeights[j].Value / TotalInfluence;
					V.InfluenceBones[j] = static_cast<uint8>(SortedWeights[j].Key);
					V.InfluenceWeights[j] = FMath::Clamp(FMath::RoundToInt(NormalizedWeight * 255.0f), 1, 255);
					WeightSum += V.InfluenceWeights[j];
				}

				// Normalize to 255
				if (WeightSum != 255.0f && NumInfluences > 0)
				{
					float Scale = 255.0f / WeightSum;
					int32 ActualSum = 0;

					for (int32 j = 0; j < NumInfluences; ++j)
					{
						V.InfluenceWeights[j] = FMath::Clamp(FMath::RoundToInt(V.InfluenceWeights[j] * Scale), 1, 255);
						ActualSum += V.InfluenceWeights[j];
					}

					if (ActualSum != 255)
					{
						V.InfluenceWeights[0] += (255 - ActualSum);
						V.InfluenceWeights[0] = FMath::Clamp(FMath::RoundToInt(V.InfluenceWeights[0] * 1.0), 1, 255);
					}
				}
			}
		}
	}

}

// Current build function builds combined mesh and assigns all materials correctly
void UUMeshMergeUtility::BuildSkeletalMeshFromRawMeshDataMultiMaterial(
	/*const*/ FRawMeshData& Raw,                  // populated with MergedRaw so shuld include base and caps
	const TArray<FRawVertexData>& RawV,          // authored skin data
	const TArray<FSkeletalMaterial>& Materials,  // Materials for each section
	const TArray<int32>& TriangleMaterialIndices, // Material index per triangle
	USkeletalMesh* SkeletalMesh, const TMap<int32, FCapSkinningInfo>& CapSkinningInfos)
{
	if (!SkeletalMesh || Raw.Vertices.Num() == 0 || Raw.Triangles.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("? Invalid mesh data"));
		return;
	}

	// Build name?index map
	TMap<FName, int32> BoneNameToIndex;
	const FReferenceSkeleton& RefSkel = SkeletalMesh->GetRefSkeleton();
	for (int32 BI = 0; BI < RefSkel.GetNum(); ++BI)
	{
		BoneNameToIndex.Add(RefSkel.GetBoneName(BI), BI);
	}

	// 1) Clear out any existing LODs and start fresh
	FSkeletalMeshModel* MeshModel = SkeletalMesh->GetImportedModel();
	MeshModel->LODModels.Empty();
	MeshModel->LODModels.Add(new FSkeletalMeshLODModel());
	FSkeletalMeshLODModel& LODModel = MeshModel->LODModels[0];

	LODModel.Sections.Empty();
	LODModel.IndexBuffer.Empty();

	// Count triangles per material in the array
	TMap<int32, int32> MaterialCounts;
	for (int32 MatIdx : TriangleMaterialIndices)
	{
		MaterialCounts.FindOrAdd(MatIdx)++;
	}

	// 2) Group triangles by material
	TMap<int32, TArray<int32>> MaterialToTriangles;
	for (int32 TriIdx = 0; TriIdx < Raw.Triangles.Num(); ++TriIdx)
	{
		int32 MatIdx = TriangleMaterialIndices.IsValidIndex(TriIdx) ? TriangleMaterialIndices[TriIdx] : 0;
		MaterialToTriangles.FindOrAdd(MatIdx).Add(TriIdx);
	}

	// 3) Extract geometry arrays (shared by all sections)
	TArray<FVector>   Positions; Positions.Reserve(Raw.Vertices.Num());
	TArray<FVector>   Normals = Raw.Normals;
	TArray<FVector>   Tangents = Raw.Tangents;
	TArray<FVector2D> UVs = Raw.UVs;
	TArray<FColor>    Colors = Raw.Colors;
	
	for (const FRawVertexData& V : Raw.Vertices)
	{
		Positions.Add(FVector(V.Position));
	}

	// 4) Create a SINGLE master section with empty BoneMap initially
	FSkelMeshSection MasterSection;
	MasterSection.MaterialIndex = 0;
	MasterSection.BaseVertexIndex = 0;
	MasterSection.NumVertices = Raw.Vertices.Num();
	MasterSection.bUse16BitBoneIndex = true;
	MasterSection.SoftVertices.SetNumZeroed(Raw.Vertices.Num());
	MasterSection.BoneMap.Reset(); // Start empty!

	// Run AutoSkinSection2 - let IT build the BoneMap
	TMap<int32, FCapSkinningInfo> TempInfos = CapSkinningInfos;
	AutoSkinSection2(RefSkel, RawV, Positions, Normals, Tangents, UVs, Colors, LODModel, MasterSection, TempInfos, 5.f, MAX_TOTAL_INFLUENCES);

	// NOW harvest the SharedBoneMap from what AutoSkinSection2 actually created
	TArray<uint16> SharedBoneMap = MasterSection.BoneMap; // Use what AutoSkin built!

	SharedBoneMap.Sort();
	
	// 5) Create separate sections for each material
	int32 GlobalIndexOffset = 0;

	for (const auto& MatPair : MaterialToTriangles)
	{
		int32 MaterialIndex = MatPair.Key;
		const TArray<int32>& TriIndices = MatPair.Value;

		FSkelMeshSection Section;
		Section.MaterialIndex = MaterialIndex;
		Section.BaseVertexIndex = 0;  // All sections share the same vertex buffer
		Section.NumVertices = Raw.Vertices.Num();  // All sections can access all vertices
		Section.BaseIndex = GlobalIndexOffset;
		Section.NumTriangles = TriIndices.Num();
		Section.bUse16BitBoneIndex = true;

		// ONLY the first section gets the SoftVertices data
		if (LODModel.Sections.Num() == 0)
		{
			Section.SoftVertices = MasterSection.SoftVertices;
		}
		else
		{
			Section.SoftVertices.Empty();
		}

		// Use the shared bone map
		Section.BoneMap = SharedBoneMap;

		// Add triangles for this material
		for (int32 TriIdx : TriIndices)
		{
			if (Raw.Triangles.IsValidIndex(TriIdx))
			{
				const FIntVector& Tri = Raw.Triangles[TriIdx];
				LODModel.IndexBuffer.Add(Tri.X);
				LODModel.IndexBuffer.Add(Tri.Y);
				LODModel.IndexBuffer.Add(Tri.Z);
			}
		}

		GlobalIndexOffset = LODModel.IndexBuffer.Num();

		LODModel.Sections.Add(Section);
	}
	// 6) Finalize LOD metadata (following your working pattern)
	LODModel.RequiredBones.Reset();
	for (int32 BoneIdx : SharedBoneMap)
	{
		LODModel.RequiredBones.Add(BoneIdx);
	}
	LODModel.ActiveBoneIndices = LODModel.RequiredBones;

	// CRITICAL: NumVertices should equal the TOTAL SoftVertices count, not vertex count
	int32 TotalSoftVertices = 0;
	for (const FSkelMeshSection& Sec : LODModel.Sections)
	{
		TotalSoftVertices += Sec.SoftVertices.Num();
	}
	LODModel.NumVertices = TotalSoftVertices;
	LODModel.NumTexCoords = 1;

	// Sanity check
	if (LODModel.NumVertices != TotalSoftVertices)
	{
		UE_LOG(LogTemp, Error, TEXT("? LODModel.NumVertices (%d) != TotalSoftVertices (%d)"), LODModel.NumVertices, TotalSoftVertices);
		return;
	}

	// 7) Set materials
	SkeletalMesh->SetMaterials(Materials);

	// 8) Bounds and LODInfo (following your working pattern)
	FBox Box(ForceInit);
	for (const FRawVertexData& V : Raw.Vertices)
	{
		Box += FVector(V.Position);
	}
	SkeletalMesh->SetImportedBounds(FBoxSphereBounds(Box));

	if (SkeletalMesh->GetLODInfoArray().Num() == 0)
	{
		FSkeletalMeshLODInfo LODInfo;
		LODInfo.ScreenSize.Default = 1.0f;
		LODInfo.LODHysteresis = 0.02f;
		LODInfo.ReductionSettings.NumOfTrianglesPercentage = 1.0f;
		LODInfo.ReductionSettings.NumOfVertPercentage = 1.0f;
		LODInfo.ReductionSettings.MaxDeviationPercentage = 0.0f;
		SkeletalMesh->AddLODInfo(LODInfo);
	}

	SkeletalMesh->SetHasVertexColors(true);
	SkeletalMesh->SetEnablePerPolyCollision(false);

	// 9) Finalize (following your working pattern)
	SkeletalMesh->CalculateInvRefMatrices();
	SkeletalMesh->PostEditChange();
	SkeletalMesh->InitResources();

}	

// Function to update cap skinning info after welding
void UUMeshMergeUtility::UpdateCapSkinningIndicesAfterWelding(
	TMap<int32, FCapSkinningInfo>& CapSkinningInfos,
	const FWeldedMeshData& WeldedData)
{
	
	for (auto& CapPair : CapSkinningInfos)
	{
		FCapSkinningInfo& CapInfo = CapPair.Value;
		int32 CapIndex = CapPair.Key;

		// Find the new welded vertex indices for this cap
		TSet<int32> WeldedVerticesForThisCap;

		// Check all original vertices in this cap's range
		for (int32 OriginalVertIdx = CapInfo.StartVertexIndex; OriginalVertIdx <= CapInfo.EndVertexIndex; ++OriginalVertIdx)
		{
			if (WeldedData.VertexRemapping.IsValidIndex(OriginalVertIdx))
			{
				int32 WeldedVertIdx = WeldedData.VertexRemapping[OriginalVertIdx];
				WeldedVerticesForThisCap.Add(WeldedVertIdx);

			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("  Original vertex %d is out of bounds in remapping array!"), OriginalVertIdx);
			}
		}

		// Convert set to sorted array
		TArray<int32> SortedWeldedVertices = WeldedVerticesForThisCap.Array();
		SortedWeldedVertices.Sort();

		if (SortedWeldedVertices.Num() > 0)
		{
			// Store all the welded vertices that belong to this cap
			CapInfo.StartVertexIndex = SortedWeldedVertices[0];
			CapInfo.EndVertexIndex = SortedWeldedVertices.Last();

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cap[%d] has no vertices after welding!"), CapIndex);
			CapInfo.StartVertexIndex = 0;
			CapInfo.EndVertexIndex = -1; // Invalid range
		}
	}
}

// Alternative approach - store specific vertex indices for each cap
void UUMeshMergeUtility::AutoSkinCapVerticesWithIndices(
	const FReferenceSkeleton& RefSkeleton,
	const TArray<FRawVertexData>& RawVertexData,
	const TArray<FVector>& Vertices,
	FSkeletalMeshLODModel& LODModel,
	FSkelMeshSection& Section,
	const TMap<int32, FCapSkinningInfo>& CapSkinningInfos,
	const FWeldedMeshData& WeldedData,
	int32 MaxInfluences = MAX_TOTAL_INFLUENCES)
{
	//UE_LOG(LogTemp, Warning, TEXT("Running AutoSkinCapVerticesWithIndices for %d caps"), CapSkinningInfos.Num());

	// Build compact bone index map
	TMap<int32, int32> BoneToCompact;
	for (int32 i = 0; i < Section.BoneMap.Num(); ++i)
	{
		BoneToCompact.Add(Section.BoneMap[i], i);
	}

	// For each cap, find ALL welded vertices that came from that cap's original range
	for (const auto& CapPair : CapSkinningInfos)
	{
		const FCapSkinningInfo& CapInfo = CapPair.Value;
		int32 CapIndex = CapPair.Key;

		UE_LOG(LogTemp, Warning, TEXT("Processing Cap[%d] original range %d-%d"),
			CapIndex, CapInfo.StartVertexIndex, CapInfo.EndVertexIndex);

		// Find ALL welded vertices that originated from this cap
		TSet<int32> CapWeldedVertices;
		for (int32 OriginalVertIdx = CapInfo.StartVertexIndex; OriginalVertIdx <= CapInfo.EndVertexIndex; ++OriginalVertIdx)
		{
			if (WeldedData.VertexRemapping.IsValidIndex(OriginalVertIdx))
			{
				int32 WeldedVertIdx = WeldedData.VertexRemapping[OriginalVertIdx];
				CapWeldedVertices.Add(WeldedVertIdx);
			}
		}

		TArray<int32> CapVertices = CapWeldedVertices.Array();
		CapVertices.Sort();

		// Process each welded vertex that belongs to this cap
		for (int32 WeldedVertIdx : CapVertices)
		{
			if (WeldedVertIdx >= 0 && WeldedVertIdx < Section.SoftVertices.Num())
			{
				FSoftSkinVertex& V = Section.SoftVertices[WeldedVertIdx];

				// CLEAR existing skinning first
				for (int32 j = 0; j < MaxInfluences; ++j)
				{
					V.InfluenceBones[j] = 0;
					V.InfluenceWeights[j] = 0;
				}

				UE_LOG(LogTemp, Warning, TEXT("Force re-skinning Cap[%d] welded vertex %d"), CapIndex, WeldedVertIdx);

				// Apply cap-specific skinning
				ApplyCapSpecificSkinning(V, WeldedVertIdx, CapInfo, CapIndex, RefSkeleton, RawVertexData,
					Vertices, Section, BoneToCompact, RawVertexData.Num(), MaxInfluences);
			}
		}
	}
}

// Helper function to apply specific cap skinning methods
void UUMeshMergeUtility::ApplyCapSpecificSkinning(
	FSoftSkinVertex& Vertex,
	int32 VertexIndex,
	const FCapSkinningInfo& CapInfo,
	int32 CapIndex,
	const FReferenceSkeleton& RefSkeleton,
	const TArray<FRawVertexData>& RawVertexData,
	const TArray<FVector>& Vertices,
	const FSkelMeshSection& Section,
	const TMap<int32, int32>& BoneToCompact,
	int32 AuthoredVerts,
	int32 MaxInfluences)
{
	if (CapInfo.SkinningMethod == ESkinningType::RigidClosestBone)
	{
		// RIGID CLOSEST BONE METHOD
		FVector VertexPos = Vertices[VertexIndex];
		float MinDistance = FLT_MAX;
		int32 ClosestBoneCompactIdx = 0;

		// Check all bones in our BoneMap
		for (int32 BoneMapIdx = 0; BoneMapIdx < Section.BoneMap.Num(); ++BoneMapIdx)
		{
			int32 RefSkeletonIdx = Section.BoneMap[BoneMapIdx];

			// Get bone position from reference skeleton
			FTransform BoneTransform = RefSkeleton.GetRefBonePose()[RefSkeletonIdx];

			// Transform relative to parent bones
			int32 ParentIdx = RefSkeleton.GetParentIndex(RefSkeletonIdx);
			while (ParentIdx != INDEX_NONE)
			{
				BoneTransform = BoneTransform * RefSkeleton.GetRefBonePose()[ParentIdx];
				ParentIdx = RefSkeleton.GetParentIndex(ParentIdx);
			}

			FVector BonePos = BoneTransform.GetLocation();
			float Distance = FVector::Dist(VertexPos, BonePos);

			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestBoneCompactIdx = BoneMapIdx;
			}
		}

		// Give 100% weight to closest bone
		Vertex.InfluenceBones[0] = static_cast<uint8>(ClosestBoneCompactIdx);
		Vertex.InfluenceWeights[0] = 255;

		int32 RefBoneIdx = Section.BoneMap[ClosestBoneCompactIdx];
		
	}
	else // ESkinningType::ProximityBlending
	{
		// PROXIMITY BLENDING METHOD
		TMap<int32, float> Accum;

		for (int32 k = 0; k < AuthoredVerts; ++k)
		{
			const FSoftSkinVertex& Nbr = Section.SoftVertices[k];
			if (Nbr.InfluenceWeights[0] == 0) continue;

			float Dist = FVector::Dist(Vertices[VertexIndex], Vertices[k]);
			if (Dist > CapInfo.ProximityRadius) continue;

			float Falloff = 1.f - (Dist / CapInfo.ProximityRadius);
			for (int32 inf = 0; inf < MaxInfluences; ++inf)
			{
				uint8 W = Nbr.InfluenceWeights[inf];
				if (W == 0) break;
				Accum.FindOrAdd(Nbr.InfluenceBones[inf]) += (W / 255.f) * Falloff;
			}
		}

		Accum.ValueSort([](float A, float B) { return A > B; });

		float TotalW = 0.f;
		int32 idx = 0;
		for (auto& Pair : Accum)
		{
			if (idx >= MaxInfluences) break;
			Vertex.InfluenceBones[idx] = static_cast<uint8>(Pair.Key);
			Vertex.InfluenceWeights[idx] = FMath::Clamp(FMath::RoundToInt(Pair.Value * 255.f), 1, 255);
			TotalW += Vertex.InfluenceWeights[idx++];
		}

		if (idx == 0)
		{
			Vertex.InfluenceBones[0] = 0;
			Vertex.InfluenceWeights[0] = 255;
		}
		else
		{
			float Scale = 255.f / TotalW;
			int32 Sum = 0;
			for (int32 j = 0; j < idx; ++j)
			{
				Vertex.InfluenceWeights[j] = FMath::Clamp(FMath::RoundToInt(Vertex.InfluenceWeights[j] * Scale), 1, 255);
				Sum += Vertex.InfluenceWeights[j];
			}
			if (Sum != 255)
			{
				Vertex.InfluenceWeights[0] = FMath::Clamp(Vertex.InfluenceWeights[0] + (255 - Sum), 1, 255);
			}
		}
	}
}

// Updated welded mesh builder that uses the new cap skinning system
void UUMeshMergeUtility::BuildSkeletalMeshFromRawMeshDataMultiMaterialWelded(
	const FRawMeshData& Raw,
	const TArray<FRawVertexData>& RawV,
	const TArray<FSkeletalMaterial>& Materials,
	const TArray<int32>& TriangleMaterialIndices,
	USkeletalMesh* SkeletalMesh,
	const TMap<int32, FCapSkinningInfo>& CapSkinningInfos,
	float WeldThreshold = 0.001f)
{
	if (!SkeletalMesh || Raw.Vertices.Num() == 0 || Raw.Triangles.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid mesh data"));
		return;
	}

	// Build bone name to index map
	const FReferenceSkeleton& RefSkel = SkeletalMesh->GetRefSkeleton();

	// 1) WELD DUPLICATE VERTICES
	FWeldedMeshData WeldedData = WeldDuplicateVertices(Raw, RawV, WeldThreshold);

	// 2) UPDATE CAP SKINNING INDICES FOR WELDED VERTICES
	TMap<int32, FCapSkinningInfo> UpdatedCapSkinningInfos = CapSkinningInfos;
	UpdateCapSkinningIndicesAfterWelding(UpdatedCapSkinningInfos, WeldedData);

	// 3) Clear out any existing LODs and start fresh
	FSkeletalMeshModel* MeshModel = SkeletalMesh->GetImportedModel();
	MeshModel->LODModels.Empty();
	MeshModel->LODModels.Add(new FSkeletalMeshLODModel());
	FSkeletalMeshLODModel& LODModel = MeshModel->LODModels[0];

	LODModel.Sections.Empty();
	LODModel.IndexBuffer.Empty();

	// 4) Group triangles by material
	TMap<int32, TArray<int32>> MaterialToTriangles;
	for (int32 TriIdx = 0; TriIdx < WeldedData.WeldedTriangles.Num(); ++TriIdx)
	{
		int32 MatIdx = TriangleMaterialIndices.IsValidIndex(TriIdx) ? TriangleMaterialIndices[TriIdx] : 0;
		MaterialToTriangles.FindOrAdd(MatIdx).Add(TriIdx);
	}

	// 5) Create master section with WELDED data and run main skinning (for authored vertices only)
	FSkelMeshSection MasterSection;
	MasterSection.MaterialIndex = 0;
	MasterSection.BaseVertexIndex = 0;
	MasterSection.NumVertices = WeldedData.GetWeldedVertexCount();
	MasterSection.bUse16BitBoneIndex = true;
	MasterSection.SoftVertices.SetNumZeroed(WeldedData.GetWeldedVertexCount());
	MasterSection.BoneMap.Reset();

	// Run main AutoSkinSection2 (this handles authored vertices but leaves caps unskinned)
	TMap<int32, FCapSkinningInfo> TempInfos; // Pass empty - we'll handle caps separately
	AutoSkinSection2(RefSkel, WeldedData.WeldedRawVertexData,
		WeldedData.WeldedPositions, WeldedData.WeldedNormals,
		WeldedData.WeldedTangents, WeldedData.WeldedUVs, WeldedData.WeldedColors,
		LODModel, MasterSection, TempInfos, 5.f, MAX_TOTAL_INFLUENCES);

	// 6) NOW RUN CAP-SPECIFIC SKINNING (using the welded vertex indices directly)
	AutoSkinCapVerticesWithIndices(RefSkel, WeldedData.WeldedRawVertexData, WeldedData.WeldedPositions,
		LODModel, MasterSection, CapSkinningInfos, WeldedData, MAX_TOTAL_INFLUENCES);

	// Get the shared bone map
	TArray<uint16> SharedBoneMap = MasterSection.BoneMap;
	SharedBoneMap.Sort();

	// 7) Create separate sections for each material
	int32 GlobalIndexOffset = 0;

	for (const auto& MatPair : MaterialToTriangles)
	{
		int32 MaterialIndex = MatPair.Key;
		const TArray<int32>& TriIndices = MatPair.Value;

		FSkelMeshSection Section;
		Section.MaterialIndex = MaterialIndex;
		Section.BaseVertexIndex = 0;
		Section.NumVertices = WeldedData.GetWeldedVertexCount();
		Section.BaseIndex = GlobalIndexOffset;
		Section.NumTriangles = TriIndices.Num();
		Section.bUse16BitBoneIndex = true;

		// Only the FIRST section gets the SoftVertices data
		if (LODModel.Sections.Num() == 0)
		{
			Section.SoftVertices = MasterSection.SoftVertices;
		}
		else
		{
			Section.SoftVertices.Empty();
		}

		// Set bone map
		Section.BoneMap.Empty();
		for (int32 BoneIdx : SharedBoneMap)
		{
			Section.BoneMap.Add((FBoneIndexType)BoneIdx);
		}

		// Add triangles to index buffer
		for (int32 TriIdx : TriIndices)
		{
			if (WeldedData.WeldedTriangles.IsValidIndex(TriIdx))
			{
				const FIntVector& Tri = WeldedData.WeldedTriangles[TriIdx];
				LODModel.IndexBuffer.Add(Tri.X);
				LODModel.IndexBuffer.Add(Tri.Y);
				LODModel.IndexBuffer.Add(Tri.Z);
			}
		}

		GlobalIndexOffset = LODModel.IndexBuffer.Num();
		LODModel.Sections.Add(Section);
	}

	// 8) Finalize LOD metadata
	LODModel.RequiredBones.Reset();
	for (int32 BoneIdx : SharedBoneMap)
	{
		LODModel.RequiredBones.Add(BoneIdx);
	}
	LODModel.ActiveBoneIndices = LODModel.RequiredBones;

	int32 TotalSoftVertices = 0;
	for (const FSkelMeshSection& Sec : LODModel.Sections)
	{
		TotalSoftVertices += Sec.SoftVertices.Num();
	}
	LODModel.NumVertices = TotalSoftVertices;
	LODModel.NumTexCoords = 1;

	// 9) Set materials and bounds
	SkeletalMesh->SetMaterials(Materials);

	FBox Box(ForceInit);
	for (const FVector& Pos : WeldedData.WeldedPositions)
	{
		Box += Pos;
	}
	SkeletalMesh->SetImportedBounds(FBoxSphereBounds(Box));

	// 10) LODInfo and finalization
	if (SkeletalMesh->GetLODInfoArray().Num() == 0)
	{
		FSkeletalMeshLODInfo LODInfo;
		LODInfo.ScreenSize.Default = 1.0f;
		LODInfo.LODHysteresis = 0.02f;
		LODInfo.ReductionSettings.NumOfTrianglesPercentage = 1.0f;
		LODInfo.ReductionSettings.NumOfVertPercentage = 1.0f;
		LODInfo.ReductionSettings.MaxDeviationPercentage = 0.0f;
		SkeletalMesh->AddLODInfo(LODInfo);
	}

	SkeletalMesh->SetHasVertexColors(true);
	SkeletalMesh->SetEnablePerPolyCollision(false);
	SkeletalMesh->CalculateInvRefMatrices();
	SkeletalMesh->PostEditChange();
	SkeletalMesh->InitResources();

}

// Updated welded mesh builder that uses the new cap skinning system
void UUMeshMergeUtility::BuildStaticMeshFromRawMeshDataMultiMaterialWelded(
	const FRawMeshData& Raw,
	const TArray<FRawVertexData>& RawV,
	const TArray<FSkeletalMaterial>& Materials,
	const TArray<int32>& TriangleMaterialIndices,
	UStaticMesh* FinalMesh,
	const TMap<int32, FCapSkinningInfo>& CapSkinningInfos,
	float WeldThreshold = 0.001f)
{
	if (/*!FinalMesh ||*/ Raw.Vertices.Num() == 0 || Raw.Triangles.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Raw input data"));
		return;
	}

	// 1) WELD DUPLICATE VERTICES
	FWeldedMeshData WeldedData = PopulateMeshDataWithoutWelding(Raw, RawV);  /*WeldDuplicateVertices(Raw, RawV, WeldThreshold);*/

	FMeshDescription MeshDescription;
	FStaticMeshAttributes Attributes(MeshDescription);
	Attributes.Register();
	
	FMeshDescriptionBuilder MeshBuilder;
	MeshBuilder.SetMeshDescription(&MeshDescription);
	MeshBuilder.EnablePolyGroups();

	TVertexAttributesRef<FVector3f> VertexPositions = Attributes.GetVertexPositions();
	
	TVertexInstanceAttributesRef<FVector3f> Normals = Attributes.GetVertexInstanceNormals();
	TVertexInstanceAttributesRef<FVector3f> Tangents = Attributes.GetVertexInstanceTangents();
	TVertexInstanceAttributesRef<float> BinormalSigns = Attributes.GetVertexInstanceBinormalSigns();
	TVertexInstanceAttributesRef<FVector2f> UVs = Attributes.GetVertexInstanceUVs();

	TArray<FVertexID> VertexIDs;
	for (const FVector& Position : WeldedData.WeldedPositions)
	{
		FVertexID VertexID = MeshDescription.CreateVertex();
		VertexPositions[VertexID] = FVector3f(Position);
		VertexIDs.Add(VertexID);
	}

	auto CreateTriangle = [&](const FIntVector& Tri)
		{
			return TArray<FVertexInstanceID>{
				MeshDescription.CreateVertexInstance(VertexIDs[Tri.X]),
				MeshDescription.CreateVertexInstance(VertexIDs[Tri.Y]),
				MeshDescription.CreateVertexInstance(VertexIDs[Tri.Z])
			};
		};

	// Map material indices to polygon groups
	TMap<UMaterialInterface*, FPolygonGroupID> MaterialToPolygonGroup;

	for (const FSkeletalMaterial& SkeletalMat : Materials)
	{
		UMaterialInterface* MatInterface = SkeletalMat.MaterialInterface;
		if (!MatInterface) continue;

		FPolygonGroupID PolygonGroupID = MeshDescription.CreatePolygonGroup();
		MaterialToPolygonGroup.Add(MatInterface, PolygonGroupID);
	}

	TArray<FVertexInstanceID> VertexInstanceIDs;
	
	for (int32 i = 0; i < WeldedData.WeldedTriangles.Num(); i++)
	{
		int32 MaterialIndex = TriangleMaterialIndices.IsValidIndex(i) ? TriangleMaterialIndices[i] : 0;
		UMaterialInterface* MatInterface = Materials.IsValidIndex(MaterialIndex)
			? Materials[MaterialIndex].MaterialInterface
			: nullptr;
		
		FPolygonGroupID PolygonGroup = MaterialToPolygonGroup.Contains(MatInterface)
			? MaterialToPolygonGroup[MatInterface]
			: MaterialToPolygonGroup[Materials[0].MaterialInterface];
		const FIntVector& Tri = WeldedData.WeldedTriangles[i];
		TArray<FVertexInstanceID> TriangleInstances;

		for (int32 Corner = 0; Corner < 3; ++Corner)
		{
			int32 VertexIndex = Tri[Corner];
			FVertexInstanceID VI = MeshDescription.CreateVertexInstance(VertexIDs[VertexIndex]);

			Normals[VI] = FVector3f(WeldedData.WeldedNormals[VertexIndex]);
			Tangents[VI] = FVector3f(WeldedData.WeldedTangents[VertexIndex]);
			const FVector& TangentX = WeldedData.WeldedTangents[VertexIndex];
			const FVector& Normal = WeldedData.WeldedNormals[VertexIndex];
			FVector TangentY = FVector::CrossProduct(Normal, TangentX).GetSafeNormal();
			BinormalSigns[VI] = GetBasisDeterminantSign(TangentX, TangentY, Normal);
			UVs[VI] = FVector2f(WeldedData.WeldedUVs[VertexIndex]);

			TriangleInstances.Add(VI);
		}

		MeshDescription.CreatePolygon(PolygonGroup, TriangleInstances);
		
		
	}


	
	UVs.SetNumChannels(1); // Set number of UV channels

	// 2. Allocate a new LOD
	FinalMesh->SetNumSourceModels(1);
	FStaticMeshSourceModel& SrcModel = FinalMesh->GetSourceModel(0);
	SrcModel.BuildSettings.bRecomputeNormals = false;
	SrcModel.BuildSettings.bRecomputeTangents = false;

	// 3. Set the mesh description
	FinalMesh->CreateMeshDescription(0, MeshDescription);
	FinalMesh->CommitMeshDescription(0);

	
	for (const FSkeletalMaterial& Material : Materials)
	{
		if (Material.MaterialInterface)
		{
			FStaticMaterial StaticMaterial(Material.MaterialInterface, Material.MaterialSlotName, Material.ImportedMaterialSlotName);
			FinalMesh->GetStaticMaterials().Add(StaticMaterial);
		}
	}

	// 5. Build render data
	FinalMesh->Build(false); // false = don't mark dirty
	FinalMesh->PostEditChange();

	FAssetRegistryModule::AssetCreated(FinalMesh);
	FinalMesh->MarkPackageDirty();
	
}

void UUMeshMergeUtility::BuildStaticMeshFromProceduralMeshWelded(
	const FRawMeshData& Raw,
	UMaterialInterface*& Materials,
	const TArray<int32>& TriangleMaterialIndices,
	UStaticMesh* FinalMesh,
	float WeldThreshold = 0.001f)
{
	if (!FinalMesh || Raw.Vertices.Num() == 0 || Raw.Triangles.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Raw input data"));
		return;
	}

	// 1. Create MeshDescription
	FMeshDescription MeshDescription;
	FStaticMeshAttributes Attributes(MeshDescription);
	Attributes.Register();

	FMeshDescriptionBuilder MeshBuilder;
	MeshBuilder.SetMeshDescription(&MeshDescription);
	MeshBuilder.EnablePolyGroups();

	TVertexAttributesRef<FVector3f> VertexPositions = Attributes.GetVertexPositions();
	TVertexInstanceAttributesRef<FVector3f> Normals = Attributes.GetVertexInstanceNormals();
	TVertexInstanceAttributesRef<FVector3f> Tangents = Attributes.GetVertexInstanceTangents();
	TVertexInstanceAttributesRef<float> BinormalSigns = Attributes.GetVertexInstanceBinormalSigns();
	TVertexInstanceAttributesRef<FVector2f> UVs = Attributes.GetVertexInstanceUVs();
	UVs.SetNumChannels(1);

	// 2. Create vertices
	TArray<FVertexID> VertexIDs; 
	TArray<FRawVertexData> verts = Raw.Vertices;
	TArray<FVector> TempVecs;
	for (FRawVertexData vec : verts)
	{
		TempVecs.Add(FVector(vec.Position));
	}
	for (const FVector& Position : TempVecs)
	{
		FVertexID VertexID = MeshDescription.CreateVertex();
		VertexPositions[VertexID] = FVector3f(Position);
		VertexIDs.Add(VertexID);
	}

	// 3. Create a single polygon group
	FPolygonGroupID PolygonGroupID = MeshDescription.CreatePolygonGroup();

	// 4. Create triangles and assign attributes
	for (int32 i = 0; i < Raw.Triangles.Num(); ++i)
	{
		const FIntVector& Tri = Raw.Triangles[i];
		TArray<FVertexInstanceID> TriangleInstances;

		for (int32 Corner = 0; Corner < 3; ++Corner)
		{
			int32 VertexIndex = Tri[Corner];
			FVertexInstanceID VI = MeshDescription.CreateVertexInstance(VertexIDs[VertexIndex]);

			Normals[VI] = FVector3f(Raw.Normals[VertexIndex]);
			Tangents[VI] = FVector3f(Raw.Tangents[VertexIndex]);

			const FVector& TangentX = Raw.Tangents[VertexIndex];
			const FVector& Normal = Raw.Normals[VertexIndex];
			FVector TangentY = FVector::CrossProduct(Normal, TangentX).GetSafeNormal();
			BinormalSigns[VI] = GetBasisDeterminantSign(TangentX, TangentY, Normal);

			UVs[VI] = FVector2f(Raw.UVs[VertexIndex]);

			TriangleInstances.Add(VI);
		}

		MeshDescription.CreatePolygon(PolygonGroupID, TriangleInstances);
	}

	// 5. Setup LOD
	FinalMesh->SetNumSourceModels(1);
	FStaticMeshSourceModel& SrcModel = FinalMesh->GetSourceModel(0);
	SrcModel.BuildSettings.bRecomputeNormals = false;
	SrcModel.BuildSettings.bRecomputeTangents = false;

	// 6. Commit mesh description
	FinalMesh->CreateMeshDescription(0, MeshDescription);
	FinalMesh->CommitMeshDescription(0);

	// 7. Assign single material
	if (Materials)
	{
		FStaticMaterial StaticMaterial(Materials);
		FinalMesh->GetStaticMaterials().Add(StaticMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Material is null, mesh will have no material assigned."));
	}

	// 8. Finalize mesh
	FinalMesh->Build(false); // false = don't mark dirty
	FinalMesh->PostEditChange();

	FAssetRegistryModule::AssetCreated(FinalMesh);
	FinalMesh->MarkPackageDirty();

	//UE_LOG(LogTemp, Warning, TEXT("BuildStaticMeshFromProceduralMesh complete"));
}

void UUMeshMergeUtility::BuildStaticMeshFromReverseCaps(int32 index, TMap<int32, UProceduralMeshComponent*> ReverseCaps)
{
	TArray<UProceduralMeshComponent*> Comp;
	FRawMeshData OutRaw;
	TArray<int32> MaterialToTris;
	UMaterialInterface* mat = ReverseCaps[index]->GetMaterial(0);
	FString TempPath = "/Game/TempMergedStaticMeshes";
	FString AssetName = "MergedLimbMesh";

	UPackage* Package = CreatePackage(*FString::Printf(TEXT("%s/%s"), *TempPath, *AssetName));
	Package->FullyLoad();

	UStaticMesh* NewMesh = NewObject<UStaticMesh>(
		Package,
		*AssetName,
		RF_Public | RF_Standalone
	);	
	Comp.Add(ReverseCaps[index]);
	ConvertProceduralMeshToRawMeshData(Comp, OutRaw);
	BuildStaticMeshFromProceduralMeshWelded(OutRaw, mat, MaterialToTris, NewMesh, 0.001f);
	PreviewMeshInPersona(NewMesh);

}

// Blend the welded vertex influences - seems to give weights to cap verts when we dont want it
void UUMeshMergeUtility::BlendVertexInfluences(FRawVertexData& ExistingVert, const FRawVertexData& NewVert)
{
	// Build a map of all bone influences from both vertices
	TMap<int32, float> CombinedInfluences;

	// Add existing influences
	for (int32 i = 0; i < ExistingVert.BoneInfluences.Num() && i < ExistingVert.BoneWeights.Num(); ++i)
	{
		if (ExistingVert.BoneInfluences[i] != INDEX_NONE && ExistingVert.BoneWeights[i] > 0.0f)
		{
			CombinedInfluences.FindOrAdd(ExistingVert.BoneInfluences[i]) += ExistingVert.BoneWeights[i] * 0.5f;
		}
	}

	// Add new influences
	for (int32 i = 0; i < NewVert.BoneInfluences.Num() && i < NewVert.BoneWeights.Num(); ++i)
	{
		if (NewVert.BoneInfluences[i] != INDEX_NONE && NewVert.BoneWeights[i] > 0.0f)
		{
			CombinedInfluences.FindOrAdd(NewVert.BoneInfluences[i]) += NewVert.BoneWeights[i] * 0.5f;
		}
	}

	// Calculate total weight for normalization
	float TotalWeight = 0.0f;
	for (const auto& Pair : CombinedInfluences)
	{
		TotalWeight += Pair.Value;
	}

	// Clear existing arrays and rebuild with blended influences
	ExistingVert.BoneInfluences.Empty();
	ExistingVert.BoneWeights.Empty();

	if (TotalWeight > 0.0f)
	{
		// Sort influences by weight (strongest first) and normalize
		TArray<TPair<int32, float>> SortedInfluences;
		for (const auto& Pair : CombinedInfluences)
		{
			float NormalizedWeight = Pair.Value / TotalWeight;
			if (NormalizedWeight > 0.001f) // Skip very small influences
			{
				SortedInfluences.Add(TPair<int32, float>(Pair.Key, NormalizedWeight));
			}
		}

		// Sort by weight (descending)
		SortedInfluences.Sort([](const TPair<int32, float>& A, const TPair<int32, float>& B) {
			return A.Value > B.Value;
			});

		// Take up to MAX_TOTAL_INFLUENCES strongest influences
		int32 MaxInfluences = FMath::Min(SortedInfluences.Num(), MAX_TOTAL_INFLUENCES);
		ExistingVert.BoneInfluences.Reserve(MaxInfluences);
		ExistingVert.BoneWeights.Reserve(MaxInfluences);

		for (int32 i = 0; i < MaxInfluences; ++i)
		{
			ExistingVert.BoneInfluences.Add(SortedInfluences[i].Key);
			ExistingVert.BoneWeights.Add(SortedInfluences[i].Value);
		}
	}

	// Merge bone names
	for (const FName& BoneName : NewVert.BoneNames)
	{
		ExistingVert.BoneNames.AddUnique(BoneName);
	}
}

// Welding function - modular and reusable
FWeldedMeshData UUMeshMergeUtility::WeldDuplicateVertices(
	const FRawMeshData& Raw,
	const TArray<FRawVertexData>& RawV,
	float WeldThreshold = 0.001f)
{
	FWeldedMeshData WeldedData;

	// Initialize remapping array
	WeldedData.VertexRemapping.SetNum(Raw.Vertices.Num());

	// Process each vertex
	for (int32 i = 0; i < Raw.Vertices.Num(); ++i)
	{
		FVector CurrentPos = FVector(Raw.Vertices[i].Position);

		// Check if this position already exists in welded array
		int32 ExistingIndex = -1;
		for (int32 j = 0; j < WeldedData.WeldedPositions.Num(); ++j)
		{
			if (FVector::Dist(WeldedData.WeldedPositions[j], CurrentPos) < WeldThreshold)
			{
				ExistingIndex = j;
				break;
			}
		}

		if (ExistingIndex >= 0)
		{
			// Vertex position already exists - merge the data
			WeldedData.VertexRemapping[i] = ExistingIndex;

			// Blend skinning data if both vertices have it
			if (i < RawV.Num() && ExistingIndex < WeldedData.WeldedRawVertexData.Num())
			{
				BlendVertexInfluences(WeldedData.WeldedRawVertexData[ExistingIndex], RawV[i]);
			}

			// For other vertex attributes, we could blend them too, but for now keep existing
			// (normals, tangents, UVs should be very similar for welded vertices anyway)
		}
		else
		{
			// New unique vertex - add to welded arrays
			int32 NewIndex = WeldedData.WeldedPositions.Num();
			WeldedData.VertexRemapping[i] = NewIndex;

			WeldedData.WeldedPositions.Add(CurrentPos);
			WeldedData.WeldedNormals.Add(i < Raw.Normals.Num() ? Raw.Normals[i] : FVector::UpVector);
			WeldedData.WeldedTangents.Add(i < Raw.Tangents.Num() ? Raw.Tangents[i] : FVector::ForwardVector);
			WeldedData.WeldedUVs.Add(i < Raw.UVs.Num() ? Raw.UVs[i] : FVector2D::ZeroVector);
			WeldedData.WeldedColors.Add(i < Raw.Colors.Num() ? Raw.Colors[i] : FColor::White);

			// Add skinning data if available, otherwise create default
			if (i < RawV.Num())
			{
				FRawVertexData VertData = RawV[i];
				VertData.RemappedIndex = NewIndex; // Update remapped index
				WeldedData.WeldedRawVertexData.Add(VertData);
			}
			else
			{
				// Create default skinning data for vertices without authored skinning
				FRawVertexData DefaultVert;
				DefaultVert.Position = FVector3f(CurrentPos);
				DefaultVert.OriginalVertexIndex = i;
				DefaultVert.RemappedIndex = NewIndex;
				DefaultVert.BoneInfluences.Empty();
				DefaultVert.BoneWeights.Empty();
				DefaultVert.BoneNames.Empty();
				WeldedData.WeldedRawVertexData.Add(DefaultVert);
			}
		}
	}

	// Remap triangle indices to use welded vertices
	WeldedData.WeldedTriangles.Reserve(Raw.Triangles.Num());
	for (const FIntVector& Triangle : Raw.Triangles)
	{
		FIntVector WeldedTri(
			WeldedData.VertexRemapping[Triangle.X],
			WeldedData.VertexRemapping[Triangle.Y],
			WeldedData.VertexRemapping[Triangle.Z]
		);
		WeldedData.WeldedTriangles.Add(WeldedTri);
	}

	UE_LOG(LogTemp, Warning, TEXT("Vertex welding complete: %d -> %d vertices (%.1f%% reduction)"),
		Raw.Vertices.Num(), WeldedData.GetWeldedVertexCount(),
		100.0f * (1.0f - (float)WeldedData.GetWeldedVertexCount() / Raw.Vertices.Num()));

	return WeldedData;
}

FWeldedMeshData UUMeshMergeUtility::PopulateMeshDataWithoutWelding(
	const FRawMeshData& Raw,
	const TArray<FRawVertexData>& RawV)
{
	FWeldedMeshData WeldedData;

	const int32 VertexCount = Raw.Vertices.Num();
	WeldedData.VertexRemapping.SetNum(VertexCount);

	for (int32 i = 0; i < VertexCount; ++i)
	{
		const FVector CurrentPos = FVector(Raw.Vertices[i].Position);
		const int32 NewIndex = WeldedData.WeldedPositions.Num();

		WeldedData.VertexRemapping[i] = NewIndex;

		WeldedData.WeldedPositions.Add(CurrentPos);
		WeldedData.WeldedNormals.Add(i < Raw.Normals.Num() ? Raw.Normals[i] : FVector::UpVector);
		WeldedData.WeldedTangents.Add(i < Raw.Tangents.Num() ? Raw.Tangents[i] : FVector::ForwardVector);
		WeldedData.WeldedUVs.Add(i < Raw.UVs.Num() ? Raw.UVs[i] : FVector2D::ZeroVector);
		WeldedData.WeldedColors.Add(i < Raw.Colors.Num() ? Raw.Colors[i] : FColor::White);

		if (i < RawV.Num())
		{
			FRawVertexData VertData = RawV[i];
			VertData.RemappedIndex = NewIndex;
			WeldedData.WeldedRawVertexData.Add(VertData);
		}
		else
		{
			FRawVertexData DefaultVert;
			DefaultVert.Position = FVector3f(CurrentPos);
			DefaultVert.OriginalVertexIndex = i;
			DefaultVert.RemappedIndex = NewIndex;
			WeldedData.WeldedRawVertexData.Add(DefaultVert);
		}
	}

	// Copy triangles directly
	WeldedData.WeldedTriangles.Reserve(Raw.Triangles.Num());
	for (const FIntVector& Triangle : Raw.Triangles)
	{
		WeldedData.WeldedTriangles.Add(FIntVector(
			WeldedData.VertexRemapping[Triangle.X],
			WeldedData.VertexRemapping[Triangle.Y],
			WeldedData.VertexRemapping[Triangle.Z]
		));
	}

	return WeldedData;
}

// Takes the data from the main mesh and the cap meshes, and generates a final set of data for the build function
void UUMeshMergeUtility::GenerateMergedMeshData(
	const TArray<UProceduralMeshComponent*>& CapMeshComps,
	USkeletalMesh* SourceMesh,
	FRawMeshData& OutBaseRaw,
	FRawMeshData& OutMergedRaw,
	TArray<FSkeletalMaterial>& OutFinalMaterials,
	TMap<UMaterialInterface*, int32>& OutMaterialToIndex,
	TMap<int32, TArray<FIntVector>>& OutSectionTriangles,
	TMap<int32, FIntPoint>& OutSectionVertexRanges, TMap<int32, FCapSkinningInfo>& CapSkinningInfos) 
{
	if (!SourceMesh) return;

	// Step 1: Extract base skeletal mesh data
	FRawMeshData BaseRaw;

	ExtractRawMeshDataFromSkeletalMesh(SourceMesh, BaseRaw);
	
	OutBaseRaw = BaseRaw;

	// Step 2: Add base material(s)
	for (const FSkeletalMaterial& BaseMat : SourceMesh->GetMaterials())
	{
		UMaterialInterface* MatInterface = BaseMat.MaterialInterface;
		if (MatInterface && !OutMaterialToIndex.Contains(MatInterface))
		{
			int32 NewIndex = OutFinalMaterials.Num();
			OutMaterialToIndex.Add(MatInterface, NewIndex);

			FSkeletalMaterial Mat(MatInterface);
			Mat.UVChannelData.bInitialized = true;
			for (int32 ChannelIdx = 0; ChannelIdx < UE_ARRAY_COUNT(Mat.UVChannelData.LocalUVDensities); ++ChannelIdx)
			{
				Mat.UVChannelData.LocalUVDensities[ChannelIdx] = 1.0f;
			}
			OutFinalMaterials.Add(Mat);
		}
	}

	// Step 3: Add base mesh geometry
	int32 BaseVertexStart = OutMergedRaw.Vertices.Num();
	OutMergedRaw.Vertices.Append(BaseRaw.Vertices);
	OutMergedRaw.UVs.Append(BaseRaw.UVs);
	OutMergedRaw.Normals.Append(BaseRaw.Normals);
	OutMergedRaw.Tangents.Append(BaseRaw.Tangents);
	OutMergedRaw.Colors.Append(BaseRaw.Colors);

	// Offset base triangles (if needed)
	TArray<FIntVector> OffsetBaseTris = BaseRaw.Triangles;
	for (FIntVector& Tri : OffsetBaseTris)
	{
		Tri.X += BaseVertexStart;
		Tri.Y += BaseVertexStart;
		Tri.Z += BaseVertexStart;
	}
	OutMergedRaw.Triangles.Append(OffsetBaseTris);

	// Append triangle material indices
	OutMergedRaw.TriangleMaterialIndices.Append(BaseRaw.TriangleMaterialIndices);
	int32 BaseVertexCount = BaseRaw.Vertices.Num();

	// Step 4: Track base sections (FIXED - handle multiple materials)
	for (int32 TriIdx = 0; TriIdx < BaseRaw.Triangles.Num(); ++TriIdx)
	{
		// Get the material index for this triangle from the base mesh
		int32 OriginalMatIndex = BaseRaw.TriangleMaterialIndices.IsValidIndex(TriIdx) ?
			BaseRaw.TriangleMaterialIndices[TriIdx] : 0;

		// Map to our final material system
		UMaterialInterface* MatInterface = SourceMesh->GetMaterials()[OriginalMatIndex].MaterialInterface;
		int32 FinalMatIndex = OutMaterialToIndex[MatInterface];

		// Track this triangle for this material
		OutSectionTriangles.FindOrAdd(FinalMatIndex).Add(BaseRaw.Triangles[TriIdx]);
	}
	
	// Step 5: Merge cap meshes
	int32 VertexOffset = OutMergedRaw.Vertices.Num();

	for (int32 CapIndex = 0; CapIndex < CapMeshComps.Num(); ++CapIndex)
	{
		UProceduralMeshComponent* CapComp = CapMeshComps[CapIndex];
		if (!CapComp) continue;

		TArray<FRawVertexData> CapVerts;
		TArray<FIntVector> CapTris;
		TArray<FVector2D> CapUVs;
		TArray<FVector> CapNormals;
		TArray<FVector> CapTangents;
		TArray<FColor> CapColors;
		TArray<int32> CapTriMaterialIndices;

		ExtractCapRawMesh(CapComp, CapVerts, CapTris, CapUVs, CapNormals, CapTangents, CapColors, CapTriMaterialIndices);

		// POPULATE CAP SKINNING INFO HERE!
		int32 CapStartVertex = VertexOffset;
		int32 CapEndVertex = VertexOffset + CapVerts.Num() - 1;

		// Update or create the cap skinning info
		FCapSkinningInfo& CapInfo = CapSkinningInfos.FindOrAdd(CapIndex);
		CapInfo.StartVertexIndex = CapStartVertex;
		CapInfo.EndVertexIndex = CapEndVertex;

		// Set defaults only for new entries
		if (CapInfo.ProximityRadius == 0.0f) // Assuming 0.0f means uninitialized
		{
			CapInfo.SkinningMethod = ESkinningType::RigidClosestBone; // Your default
			CapInfo.ProximityRadius = 10.0f;
		}

		// Step 6: Add cap material
		
		TMap<int32, TArray<FIntVector>> SectionTris;

		for (int32 TriIdx = 0; TriIdx < CapTris.Num(); ++TriIdx)
		{
			const FIntVector& Tri = CapTris[TriIdx];
			int32 MaterialSlot = CapTriMaterialIndices.IsValidIndex(TriIdx) ? CapTriMaterialIndices[TriIdx] : 0;

			UMaterialInterface* CapMaterial = CapComp->GetMaterial(MaterialSlot);
			if (!CapMaterial) continue;

			int32 MatIndex = OutMaterialToIndex.FindOrAdd(CapMaterial, OutFinalMaterials.Num());
			if (MatIndex == OutFinalMaterials.Num()) // New material
			{
				FSkeletalMaterial Mat(CapMaterial);
				Mat.UVChannelData.bInitialized = true;
				for (int32 ChannelIdx = 0; ChannelIdx < UE_ARRAY_COUNT(Mat.UVChannelData.LocalUVDensities); ++ChannelIdx)
				{
					Mat.UVChannelData.LocalUVDensities[ChannelIdx] = 1.0f;
				}
				OutFinalMaterials.Add(Mat);
			}

			FIntVector OffsetTri(Tri.X + VertexOffset, Tri.Y + VertexOffset, Tri.Z + VertexOffset);
			SectionTris.FindOrAdd(MatIndex).Add(OffsetTri);

			//  Append triangle material index
			OutMergedRaw.Triangles.Add(OffsetTri);
			OutMergedRaw.TriangleMaterialIndices.Add(MatIndex);
		}
		// Step 7: Append geometry
		OutMergedRaw.Vertices.Append(CapVerts);
		OutMergedRaw.UVs.Append(CapUVs);
		OutMergedRaw.Normals.Append(CapNormals);
		OutMergedRaw.Tangents.Append(CapTangents);
		OutMergedRaw.Colors.Append(CapColors);

		// Step 8: Append triangles per section
		for (const TPair<int32, TArray<FIntVector>>& Pair : SectionTris)
		{
			OutSectionTriangles.FindOrAdd(Pair.Key).Append(Pair.Value);
			if (!OutSectionVertexRanges.Contains(Pair.Key))
			{
				OutSectionVertexRanges.Add(Pair.Key, FIntPoint(VertexOffset, CapVerts.Num()));
			}
		}

		VertexOffset += CapVerts.Num();

	}
}

// FIXED: Updated GenerateMergedStaticMeshData with proper base mesh section handling
void UUMeshMergeUtility::GenerateMergedStaticMeshData(
	const TArray<UProceduralMeshComponent*>& CapMeshComps,
	const FMeshBuildContext& Context,
	FRawMeshData& OutMergedRaw,
	TArray<FSkeletalMaterial>& OutFinalMaterials,
	TMap<UMaterialInterface*, int32>& OutMaterialToIndex,
	TMap<int32, TArray<FIntVector>>& OutSectionTriangles,
	TMap<int32, FIntPoint>& OutSectionVertexRanges,
	TMap<int32, FCapSkinningInfo>& CapSkinningInfos)
{
	if (Context.FinalTriangles.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Context.FinalTriangles is empty in Generate Merged Data"));
		return;
	}

	// Step 1: Extract base skeletal mesh data
	FRawMeshData BaseRaw;
	ExtractRawMeshDataFromCachedMeshContext(Context, BaseRaw);

	// Step 2: Add base material(s) and track indices
	TMap<UMaterialInterface*, int32> MaterialToFinalIndex;
	for (const FSkeletalMaterial& BaseMat : Context.MeshMaterials)
	{
		UMaterialInterface* MatInterface = BaseMat.MaterialInterface;
		if (MatInterface && !OutMaterialToIndex.Contains(MatInterface))
		{
			int32 NewIndex = OutFinalMaterials.Num();
			OutMaterialToIndex.Add(MatInterface, NewIndex);
			MaterialToFinalIndex.Add(MatInterface, NewIndex);

			FSkeletalMaterial Mat(MatInterface);
			Mat.UVChannelData.bInitialized = true;
			for (int32 ChannelIdx = 0; ChannelIdx < UE_ARRAY_COUNT(Mat.UVChannelData.LocalUVDensities); ++ChannelIdx)
			{
				Mat.UVChannelData.LocalUVDensities[ChannelIdx] = 1.0f;
			}
			OutFinalMaterials.Add(Mat);
		}
		else if (MatInterface)
		{
			MaterialToFinalIndex.Add(MatInterface, OutMaterialToIndex[MatInterface]);
		}
	}

	// Step 3: Add base mesh geometry
	int32 BaseVertexStart = 0;
	OutMergedRaw.Vertices.Append(BaseRaw.Vertices);
	OutMergedRaw.UVs.Append(BaseRaw.UVs);
	OutMergedRaw.Normals.Append(BaseRaw.Normals);
	OutMergedRaw.Tangents.Append(BaseRaw.Tangents);
	OutMergedRaw.Colors.Append(BaseRaw.Colors);

	// Step 4: Process base triangles with proper material assignments
	ProcessBaseMeshTriangles(Context, BaseRaw, MaterialToFinalIndex, OutSectionTriangles, OutSectionVertexRanges);

	// Step 5: Merge cap meshes
	int32 VertexOffset = OutMergedRaw.Vertices.Num();

	for (int32 CapIndex = 0; CapIndex < CapMeshComps.Num(); ++CapIndex)
	{
		UProceduralMeshComponent* CapComp = CapMeshComps[CapIndex];
		if (!CapComp) continue;

		TArray<FRawVertexData> CapVerts;
		TArray<FIntVector> CapTris;
		TArray<FVector2D> CapUVs;
		TArray<FVector> CapNormals;
		TArray<FVector> CapTangents;
		TArray<FColor> CapColors;
		TArray<int32> CapTriMaterialIndices;

		ExtractCapRawMesh(CapComp, CapVerts, CapTris, CapUVs, CapNormals, CapTangents, CapColors, CapTriMaterialIndices);

		// Update cap skinning info
		FCapSkinningInfo& CapInfo = CapSkinningInfos.FindOrAdd(CapIndex);
		CapInfo.StartVertexIndex = VertexOffset;
		CapInfo.EndVertexIndex = VertexOffset + CapVerts.Num() - 1;

		if (CapInfo.ProximityRadius == 0.0f)
		{
			CapInfo.SkinningMethod = ESkinningType::RigidClosestBone;
			CapInfo.ProximityRadius = 10.0f;
		}

		// Step 6: Add cap materials
		const int32 NumMaterials = CapComp->GetNumMaterials();
		for (int32 MatIdx = 0; MatIdx < NumMaterials; ++MatIdx)
		{
			UMaterialInterface* CapMaterial = CapComp->GetMaterial(MatIdx);
			if (!CapMaterial) continue;

			if (!MaterialToFinalIndex.Contains(CapMaterial))
			{
				int32 NewIndex = OutFinalMaterials.Num();
				OutMaterialToIndex.Add(CapMaterial, NewIndex);
				MaterialToFinalIndex.Add(CapMaterial, NewIndex);

				FSkeletalMaterial Mat(CapMaterial);
				Mat.UVChannelData.bInitialized = true;
				for (int32 ChannelIdx = 0; ChannelIdx < UE_ARRAY_COUNT(Mat.UVChannelData.LocalUVDensities); ++ChannelIdx)
				{
					Mat.UVChannelData.LocalUVDensities[ChannelIdx] = 1.0f;
				}
				OutFinalMaterials.Add(Mat);
			}
		}

		// Step 7: Append geometry
		OutMergedRaw.Vertices.Append(CapVerts);
		OutMergedRaw.UVs.Append(CapUVs);
		OutMergedRaw.Normals.Append(CapNormals);
		OutMergedRaw.Tangents.Append(CapTangents);
		OutMergedRaw.Colors.Append(CapColors);

		// Step 8: Process cap triangles
		for (int32 TriIdx = 0; TriIdx < CapTris.Num(); ++TriIdx)
		{
			FIntVector Tri = CapTris[TriIdx];

			// Offset triangle indices
			Tri.X += VertexOffset;
			Tri.Y += VertexOffset;
			Tri.Z += VertexOffset;

			// Determine material
			int32 MaterialSlot = CapTriMaterialIndices.IsValidIndex(TriIdx) ? CapTriMaterialIndices[TriIdx] : 0;
			UMaterialInterface* CapMaterial = CapComp->GetMaterial(MaterialSlot);

			if (CapMaterial && MaterialToFinalIndex.Contains(CapMaterial))
			{
				int32 MatIndex = MaterialToFinalIndex[CapMaterial];
				OutSectionTriangles.FindOrAdd(MatIndex).Add(Tri);

				if (!OutSectionVertexRanges.Contains(MatIndex))
				{
					OutSectionVertexRanges.Add(MatIndex, FIntPoint(VertexOffset, CapVerts.Num()));
				}
			}
		}

		VertexOffset += CapVerts.Num();
	}

	
}

void UUMeshMergeUtility::ProcessBaseMeshTriangles(
	const FMeshBuildContext& Context,
	const FRawMeshData& BaseRaw,
	const TMap<UMaterialInterface*, int32>& MaterialToFinalIndex,
	TMap<int32, TArray<FIntVector>>& OutSectionTriangles,
	TMap<int32, FIntPoint>& OutSectionVertexRanges)
{
	
	// Check which path we're taking
	if (Context.FinalMaterialIndices.Num() > 0)
	{

		int32 ProcessedTriangles = 0;
		int32 SkippedTriangles = 0;
		TMap<int32, int32> TrianglesPerMaterial;

		for (int32 TriIdx = 0; TriIdx < BaseRaw.Triangles.Num(); ++TriIdx)
		{
			const FIntVector& Triangle = BaseRaw.Triangles[TriIdx];
			int32 OriginalMatIndex = Context.FinalMaterialIndices[TriIdx];

			TrianglesPerMaterial.FindOrAdd(OriginalMatIndex)++;

			if (Context.MeshMaterials.IsValidIndex(OriginalMatIndex))
			{
				UMaterialInterface* Material = Context.MeshMaterials[OriginalMatIndex].MaterialInterface;

				if (Material && MaterialToFinalIndex.Contains(Material))
				{
					int32 FinalMatIndex = MaterialToFinalIndex[Material];

					OutSectionTriangles.FindOrAdd(FinalMatIndex).Add(Triangle);
					ProcessedTriangles++;

					if (!OutSectionVertexRanges.Contains(FinalMatIndex))
					{
						OutSectionVertexRanges.Add(FinalMatIndex, FIntPoint(0, BaseRaw.Vertices.Num()));
					}
				}
				else
				{
					SkippedTriangles++;
					
				}
			}
			else
			{
				SkippedTriangles++;
				
			}
		}

		return;
	}

	// Try BaseRaw.TriangleMaterialIndices path
	if (BaseRaw.TriangleMaterialIndices.Num() == BaseRaw.Triangles.Num())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Taking BaseRaw.TriangleMaterialIndices path"));

		// Similar processing...
		int32 ProcessedTriangles = 0;
		int32 SkippedTriangles = 0;

		for (int32 TriIdx = 0; TriIdx < BaseRaw.Triangles.Num(); ++TriIdx)
		{
			const FIntVector& Triangle = BaseRaw.Triangles[TriIdx];
			int32 MaterialIndex = BaseRaw.TriangleMaterialIndices[TriIdx];

			if (Context.MeshMaterials.IsValidIndex(MaterialIndex))
			{
				UMaterialInterface* Material = Context.MeshMaterials[MaterialIndex].MaterialInterface;
				if (Material && MaterialToFinalIndex.Contains(Material))
				{
					int32 FinalMatIndex = MaterialToFinalIndex[Material];
					OutSectionTriangles.FindOrAdd(FinalMatIndex).Add(Triangle);
					ProcessedTriangles++;

					if (!OutSectionVertexRanges.Contains(FinalMatIndex))
					{
						OutSectionVertexRanges.Add(FinalMatIndex, FIntPoint(0, BaseRaw.Vertices.Num()));
					}
				}
				else
				{
					SkippedTriangles++;
				}
			}
			else
			{
				SkippedTriangles++;
			}
		}

		
		return;
	}

	// Fallback path
	UE_LOG(LogTemp, Error, TEXT("❌ NO VALID MATERIAL INDICES FOUND - Using fallback"));
	UE_LOG(LogTemp, Error, TEXT("This should NOT happen if ExtractRawMeshDataFromCachedMeshContext is working"));

	if (Context.MeshMaterials.Num() > 0)
	{
		UMaterialInterface* FirstMaterial = Context.MeshMaterials[0].MaterialInterface;
		if (FirstMaterial && MaterialToFinalIndex.Contains(FirstMaterial))
		{
			int32 FinalMatIndex = MaterialToFinalIndex[FirstMaterial];
			OutSectionTriangles.FindOrAdd(FinalMatIndex).Append(BaseRaw.Triangles);
			OutSectionVertexRanges.Add(FinalMatIndex, FIntPoint(0, BaseRaw.Vertices.Num()));

			UE_LOG(LogTemp, Warning, TEXT("Fallback: Assigned all %d triangles to material 0 (%s)"),
				BaseRaw.Triangles.Num(), *FirstMaterial->GetName());
		}
	}
	
	// Force using Context.FinalMaterialIndices for now
	if (Context.FinalMaterialIndices.Num() > 0 && Context.FinalMaterialIndices.Num() != BaseRaw.Triangles.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("MISMATCH: FinalMaterialIndices (%d) != BaseRaw.Triangles (%d)"),
			Context.FinalMaterialIndices.Num(), BaseRaw.Triangles.Num());
	}
}

// debug function to track material assignments through the merge pipeline
static void DebugMergePipeline(
	const FMeshBuildContext& Context,
	const FRawMeshData& BaseRaw,
	const TMap<UMaterialInterface*, int32>& MaterialToFinalIndex,
	const TMap<int32, TArray<FIntVector>>& OutSectionTriangles)
{
	UE_LOG(LogTemp, Warning, TEXT("=== DEBUG MERGE PIPELINE ==="));

	// 1. Verify Context integrity
	UE_LOG(LogTemp, Warning, TEXT("Context.FinalMaterialIndices.Num() = %d"), Context.FinalMaterialIndices.Num());
	UE_LOG(LogTemp, Warning, TEXT("Context.IntVecTriangles.Num() = %d"), Context.IntVecTriangles.Num());
	UE_LOG(LogTemp, Warning, TEXT("BaseRaw.TriangleMaterialIndices.Num() = %d"), BaseRaw.TriangleMaterialIndices.Num());
	UE_LOG(LogTemp, Warning, TEXT("BaseRaw.Triangles.Num() = %d"), BaseRaw.Triangles.Num());

	// 2. Check for material index corruption in BaseRaw vs Context
	if (Context.FinalMaterialIndices.Num() > 0 && BaseRaw.TriangleMaterialIndices.Num() > 0)
	{
		int32 CheckCount = FMath::Min(Context.FinalMaterialIndices.Num(), BaseRaw.TriangleMaterialIndices.Num());
		CheckCount = FMath::Min(CheckCount, 20); // Check first 20

		UE_LOG(LogTemp, Warning, TEXT("Comparing first %d material indices:"), CheckCount);
		bool bMismatchFound = false;

		for (int32 i = 0; i < CheckCount; ++i)
		{
			int32 ContextMat = Context.FinalMaterialIndices[i];
			int32 BaseRawMat = BaseRaw.TriangleMaterialIndices[i];

			if (ContextMat != BaseRawMat)
			{
				UE_LOG(LogTemp, Error, TEXT("MISMATCH at index %d: Context=%d, BaseRaw=%d"),
					i, ContextMat, BaseRawMat);
				bMismatchFound = true;
			}
			else if (i < 5) // Log first 5 for verification
			{
				UE_LOG(LogTemp, VeryVerbose, TEXT("Index %d: Both=%d ✓"), i, ContextMat);
			}
		}

		if (!bMismatchFound)
		{
			UE_LOG(LogTemp, Warning, TEXT(" Material indices match between Context and BaseRaw"));
		}
	}

	// 3. Analyze material distribution in Context vs final sections
	TMap<int32, int32> ContextMaterialCounts;
	for (int32 MatIdx : Context.FinalMaterialIndices)
	{
		ContextMaterialCounts.FindOrAdd(MatIdx)++;
	}

	TMap<int32, int32> SectionMaterialCounts;
	for (const auto& Pair : OutSectionTriangles)
	{
		SectionMaterialCounts.Add(Pair.Key, Pair.Value.Num());
	}

	UE_LOG(LogTemp, Warning, TEXT("Material distribution comparison:"));
	UE_LOG(LogTemp, Warning, TEXT("Context material counts:"));
	for (const auto& Pair : ContextMaterialCounts)
	{
		UE_LOG(LogTemp, Warning, TEXT("  Material %d: %d triangles"), Pair.Key, Pair.Value);
	}

	UE_LOG(LogTemp, Warning, TEXT("Final section counts:"));
	for (const auto& Pair : SectionMaterialCounts)
	{
		UE_LOG(LogTemp, Warning, TEXT("  Section %d: %d triangles"), Pair.Key, Pair.Value);
	}

	// 4. Check MaterialToFinalIndex mapping integrity
	UE_LOG(LogTemp, Warning, TEXT("MaterialToFinalIndex mapping verification:"));
	for (int32 i = 0; i < Context.MeshMaterials.Num(); ++i)
	{
		UMaterialInterface* Mat = Context.MeshMaterials[i].MaterialInterface;
		if (Mat && MaterialToFinalIndex.Contains(Mat))
		{
			int32 FinalIndex = MaterialToFinalIndex[Mat];
			UE_LOG(LogTemp, Warning, TEXT("  Original Material %d (%s) -> Final Index %d"),
				i, *Mat->GetName(), FinalIndex);

			// Check if the mapping makes sense
			if (FinalIndex != i)
			{
				UE_LOG(LogTemp, Warning, TEXT("    ⚠️  Material index changed: %d -> %d"), i, FinalIndex);
			}
		}
	}
}

// Extracts all the vertices, triangles and other data needed to pass into the merged final mesh
void UUMeshMergeUtility::ExtractCapRawMesh(
	UProceduralMeshComponent* CapComp,
	TArray<FRawVertexData>& OutVertices,
	TArray<FIntVector>& OutTriangles,
	TArray<FVector2D>& OutUVs,
	TArray<FVector>& OutNormals,
	TArray<FVector>& OutTangents,
	TArray<FColor>& OutColors,
	TArray<int32>& OutTriangleMaterialIndices
)
{
	if (!CapComp) return;

	int32 RemapCounter = 0;
	int32 VertexOffset = OutVertices.Num();

	const int32 NumSections = CapComp->GetNumSections();

	for (int32 SectionIndex = 0; SectionIndex < NumSections; ++SectionIndex)
	{
		FProcMeshSection* Section = CapComp->GetProcMeshSection(SectionIndex);
		if (!Section) continue;

		const TArray<FProcMeshVertex>& ProcVerts = Section->ProcVertexBuffer;
		const TArray<uint32>& Indices = Section->ProcIndexBuffer;
		
		if (ProcVerts.Num() == 0)
		{
			UModalTaskHelper::ShowBlockingConfirmation(TEXT("ProcMesh check"),TEXT("Procedural Mesh Section has no vertices, skipping."));
			continue;
		}
		// Convert vertices
		for (int32 i = 0; i < ProcVerts.Num(); ++i)
		{
			const FProcMeshVertex& ProcVert = ProcVerts[i];

			FRawVertexData Raw;
			Raw.Position = FVector3f(ProcVert.Position);
			Raw.OriginalVertexIndex = i;
			Raw.RemappedIndex = RemapCounter++;
			Raw.CompactBoneIndex = 0;
			Raw.BoneInfluences = {};
			Raw.BoneWeights = {};
			Raw.BoneNames = {};

			OutVertices.Add(Raw);
			OutUVs.Add(FVector2D(ProcVert.UV0));
			OutNormals.Add(FVector(ProcVert.Normal));
			OutTangents.Add(FVector(ProcVert.Tangent.TangentX));
			OutColors.Add(ProcVert.Color);
		}

		// Convert triangles and tag with material index
		for (int32 i = 0; i < Indices.Num(); i += 3)
		{
			if (i + 2 < Indices.Num())
			{
				FIntVector Tri(
					static_cast<int32>(Indices[i]) + VertexOffset,
					static_cast<int32>(Indices[i + 1]) + VertexOffset,
					static_cast<int32>(Indices[i + 2]) + VertexOffset
				);

				OutTriangles.Add(Tri);
				OutTriangleMaterialIndices.Add(SectionIndex); //  Tag triangle with section/material index
			}
		}
		FString Log = CapComp->GetMaterial(0)->IsValidLowLevel() ? *CapComp->GetMaterial(0)->GetFName().ToString() : *FString::FString("Invalid");
		
		
		VertexOffset = OutVertices.Num(); // Update for next section
	}
}

void UUMeshMergeUtility::BuildFinalMeshDataFromSections(
	const FRawMeshData& InputRaw,
	const TMap<int32, TArray<FIntVector>>& SectionTriangles,
	const TArray<FSkeletalMaterial>& FinalMaterials,
	FRawMeshData& OutFinalRaw,
	TArray<int32>& OutTriangleMaterialIndices)
{
	// Copy vertex data as-is
	OutFinalRaw.Vertices = InputRaw.Vertices;
	OutFinalRaw.UVs = InputRaw.UVs;
	OutFinalRaw.Normals = InputRaw.Normals;
	OutFinalRaw.Tangents = InputRaw.Tangents;
	OutFinalRaw.Colors = InputRaw.Colors;

	// Clear triangles - we'll rebuild from sections
	OutFinalRaw.Triangles.Empty();
	OutTriangleMaterialIndices.Empty();

	// Build triangles and material indices from sections
	for (int32 MatIndex = 0; MatIndex < FinalMaterials.Num(); ++MatIndex)
	{
		if (SectionTriangles.Contains(MatIndex))
		{
			const TArray<FIntVector>& SectionTris = SectionTriangles[MatIndex];

			for (const FIntVector& Tri : SectionTris)
			{
				OutFinalRaw.Triangles.Add(Tri);
				OutTriangleMaterialIndices.Add(MatIndex);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Material %d has no triangles in SectionTriangles"), MatIndex);
		}
	}

}

void UUMeshMergeUtility::MergeMultipleCapsIntoProceduralMesh(
	const TArray<UProceduralMeshComponent*>& CapMeshComps,
	const FMeshBuildContext& Context,
	UProceduralMeshComponent* FinalMesh,
	const TMap<int32, FCapSkinningInfo>& Infos,
	bool WeldMeshes)
{
	auto Reporter = UModalTaskHelper::CreateProgressReporter(4, TEXT("Merging Caps into Procedural Mesh"));

	FRawMeshData Raw;
	TMap<int32, FCapSkinningInfo> TempInfos = Infos;
	TArray<FSkeletalMaterial> FinalMaterials;
	TMap<UMaterialInterface*, int32> MaterialToIndex;
	TMap<int32, TArray<FIntVector>> SectionTriangles;
	TMap<int32, FIntPoint> SectionVertexRanges;

	// Generate merged data with proper section tracking
	GenerateMergedStaticMeshData(CapMeshComps, Context, Raw, FinalMaterials, MaterialToIndex, SectionTriangles, SectionVertexRanges, TempInfos);
	
	Reporter->ReportProgressWithText(1.0f, TEXT("Generate Merged Mesh Data"));

	// Create temp static mesh
	FString TempPath = "/Game/TempMergedStaticMeshes";
	FString AssetName = "MergedLimbMesh";
	UPackage* Package = CreatePackage(*FString::Printf(TEXT("%s/%s"), *TempPath, *AssetName));
	Package->FullyLoad();
	UStaticMesh* NewMesh = NewObject<UStaticMesh>(
		Package,
		*AssetName,
		RF_Public | RF_Standalone
	);

	// FIXED: Build proper triangle material indices from sections
	TArray<int32> TriangleMaterialIndices;
	FRawMeshData FinalRaw;
	BuildFinalMeshDataFromSections(Raw, SectionTriangles, FinalMaterials,
		FinalRaw, TriangleMaterialIndices);

	Reporter->ReportProgressWithText(1.0f, TEXT("Built Material Indexes"));

	TArray<FRawVertexData> RawV = FinalRaw.Vertices;

	// Build the static mesh with proper sections
	BuildStaticMeshFromRawMeshDataMultiMaterialWelded(FinalRaw, RawV, FinalMaterials,
		TriangleMaterialIndices, NewMesh, TempInfos, 0.001f);
	Reporter->ReportProgressWithText(1.0f, TEXT("Built Static Mesh from Merged Data"));
	Reporter.Reset();

	PreviewMeshInPersona(NewMesh);
}

// MAIN FUNCTION CALLED BY ULIMBEXTRACTORTOOL - Takes both the Skelmesh and the Procedural Mesh Cap and merges them into a new skeletal mesh
void UUMeshMergeUtility::MergeMultipleCapsIntoSkeletalMesh(
	const TArray<UProceduralMeshComponent*>& CapMeshComps,
	const TArray<FRawVertexData>& OriginalSkelMeshCompData,
	USkeletalMesh*& OutLimbSkelMesh, // the mesh we use to get the base info from to give to the SkelMeshcomp
	const TMap<int32, FCapSkinningInfo>& Infos, bool WeldMeshes)
{
	auto Reporter = UModalTaskHelper::CreateProgressReporter(4, TEXT("Merging Caps into Skeletal Mesh"));

	FRawMeshData SkelRaw;

	TMap<int32, FCapSkinningInfo> TempInfos = Infos;
	
	Reporter->ReportProgressWithText(1.0f, TEXT("Extracted Raw Mesh Data"));

	FRawMeshData BaseRaw;
	FRawMeshData MergedRaw;
	TArray<FSkeletalMaterial> FinalMaterials;
	TMap<UMaterialInterface*, int32> MaterialToIndex;
	TMap<int32, TArray<FIntVector>> SectionTriangles;
	TMap<int32, FIntPoint> SectionVertexRanges;

	GenerateMergedMeshData(
		CapMeshComps,
		OutLimbSkelMesh,
		BaseRaw,
		MergedRaw,  // the output of combined data
		FinalMaterials,
		MaterialToIndex,
		SectionTriangles,
		SectionVertexRanges,
		TempInfos);
	UE_LOG(LogTemp, Warning, TEXT("merged mesh data"))

	Reporter->ReportProgressWithText(1.0f, TEXT("Generated Merged Mesh Data"));

	FString TempPath = "/Game/TempMergedSkelMeshes";
	FString AssetName = "MergedLimbMesh";

	UPackage* Package = CreatePackage(*FString::Printf(TEXT("%s/%s"), *TempPath, *AssetName));
	Package->FullyLoad();

	USkeletalMesh* NewMesh = NewObject<USkeletalMesh>(
		Package,
		*AssetName,
		RF_Public | RF_Standalone
	);
	
	NewMesh->GetRefSkeleton() = OutLimbSkelMesh->GetRefSkeleton();
	
	TArray<int32> TriangleMaterialIndices;
	TriangleMaterialIndices.Reserve(MergedRaw.Triangles.Num());

	// Build triangle material indices from the SectionTriangles map
	for (const auto& SectionPair : SectionTriangles)
	{
		int32 MaterialIndex = SectionPair.Key;
		const TArray<FIntVector>& Triangles = SectionPair.Value;

		// Add this material index for each triangle in this section
		for (int32 i = 0; i < Triangles.Num(); ++i)
		{
			TriangleMaterialIndices.Add(MaterialIndex);
		}
	}
	
	Reporter->ReportProgressWithText(1.0f, TEXT("Built Material Indices"));
	
		// un-welded mesh but still skinweighted.  Good for meshes that dont need to be visually perfect
	if (!WeldMeshes)
	{
		UE_LOG(LogTemp, Warning, TEXT("running build skel from raw data"))

		BuildSkeletalMeshFromRawMeshDataMultiMaterial(MergedRaw, BaseRaw.Vertices, FinalMaterials, TriangleMaterialIndices, NewMesh, TempInfos);
	}
	else  // weld mesh tris, for a 1 piece finished mesh
	{
		UE_LOG(LogTemp, Warning, TEXT("running build skel from raw data welded"))
		BuildSkeletalMeshFromRawMeshDataMultiMaterialWelded(MergedRaw, OriginalSkelMeshCompData, FinalMaterials, TriangleMaterialIndices, NewMesh, TempInfos, 0.001f);
	}

	UE_LOG(LogTemp, Warning, TEXT("after build mesh"))

	Reporter->ReportProgressWithText(1.0f, TEXT("Built Skeletal Mesh from Merged Data"));
	Reporter.Reset();

	// Optional debug visualization
	UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
	if (World)
	{
		// insert debug draw here **
	}
	UE_LOG(LogTemp, Warning, TEXT("after preview"))

	PreviewMeshInPersona(NewMesh);

	UE_LOG(LogTemp, Warning, TEXT("after preview"))

}

// This one is the function that opens out new merged mesh up in its own persona tab after generation
#if WITH_EDITOR
void UUMeshMergeUtility::PreviewMeshInPersona(UObject* Mesh)
{
	if (!Mesh || !GEditor) return;

	// Get the editor subsystem responsible for asset editors
	UAssetEditorSubsystem* AssetEditor = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	if (AssetEditor)
	{
		AssetEditor->OpenEditorForAsset(Mesh);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get UAssetEditorSubsystem"));
	}
}
#endif



