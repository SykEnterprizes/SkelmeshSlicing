
#include "ULimbExtractorTool.h"
#include "SkeletalRenderPublic.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine/TextRenderActor.h"
#include "Editor.h"
#include "Editor/EditorEngine.h"
#include <Components/TextRenderComponent.h>

#include "Math/UnrealMathUtility.h"
#include <Rendering/SkeletalMeshLODModel.h>

#include "Engine/SkeletalMesh.h"
#include "Rendering/SkeletalMeshModel.h"
#include "Rendering/SkeletalMeshLODModel.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkinWeightVertexBuffer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/Skeleton.h"
//#include "Factories/SkeletalMeshFactory.h"
#include "Materials/Material.h"
#include "Misc/Guid.h"

#include "Rendering/SkinWeightVertexBuffer.h"
#include "Algo/Sort.h"



void ULimbExtractorTool::PrintError(FString string)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *string);
}

void ULimbExtractorTool::PopulateSkelHierachy(USkeletalMesh* SkeletalMesh)
{
	if (!SkeletalMesh) return;

	BoneHierachy.Empty();
	
	USkeletalMeshComponent* SkeletalMeshComp = NewObject<USkeletalMeshComponent>(this);
	
	if (SkeletalMeshComp && SkeletalMesh)
	{
		SkeletalMeshComp->SetSkeletalMesh(SkeletalMesh);
	}

	int32 Num = SkeletalMesh->GetRefSkeleton().GetNum();
	for (int32 n = 0; n < Num; n++)
	{
		BoneHierachy.AddDefaulted(1);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("BoneHierachy length - %d"), BoneHierachy.Num());
	
	if (BoneHierachy.Num() != 0)
	{
		for (int32 i = 0; i < SkeletalMesh->GetRefSkeleton().GetNum(); i++)
		{
			BoneHierachy[i].Bone = SkeletalMesh->GetRefSkeleton().GetBoneName(i);
			//UE_LOG(LogTemp, Warning, TEXT("Bonename - %s"), *BoneHierachy[i].Bone.ToString());

			BoneHierachy[i].BoneIndex = i;
			BoneHierachy[i].BoneTransform = SkeletalMeshComp->GetSocketTransform(SkeletalMesh->GetRefSkeleton().GetBoneName(i));
			BoneHierachy[i].BoneVisibility = SkeletalMeshComp->IsBoneHiddenByName(SkeletalMesh->GetRefSkeleton().GetBoneName(i));

			TArray<FName> ChildBoneNames;
			const FReferenceSkeleton& RefSkeleton = SkeletalMesh->GetRefSkeleton();

			for (int32 BoneIndex = 0; BoneIndex < RefSkeleton.GetNum(); ++BoneIndex)
			{
				if (RefSkeleton.BoneIsChildOf(BoneIndex, i)) // Check if it's a child of 'i'
				{
					FName BoneName = RefSkeleton.GetBoneName(BoneIndex);
					ChildBoneNames.Add(BoneName);
				}
			}

			// Assign to hierarchy
			BoneHierachy[i].ChildBones = ChildBoneNames;


			// Assign to hierarchy
			BoneHierachy[i].ChildBones = ChildBoneNames;
			int32 ParentIndex = SkeletalMesh->GetRefSkeleton().GetParentIndex(i);
			if (ParentIndex != INDEX_NONE)
			{
				BoneHierachy[i].ParentBone = SkeletalMesh->GetRefSkeleton().GetBoneName(SkeletalMesh->GetRefSkeleton().GetParentIndex(i));
			}
			else
			{
				BoneHierachy[i].ParentBone = SkeletalMesh->GetRefSkeleton().GetBoneName(i);
			}

		}
	}
}

void ULimbExtractorTool::SetMinimumWeightThreshold(int32 value)
{
	MinimumWeight = value;
}

void ULimbExtractorTool::AddExclusionBones(TArray<FName> Exclusions)
{
	ExcludedBoneNames = Exclusions;
}

void ULimbExtractorTool::LoggingAndDebugSwitches(bool ShowLoops, bool MeshDiagnostic)
{
	ShowLoopDebug = ShowLoops;
	DoDiagnostic = MeshDiagnostic;
}

// The only called function in BP - does all the mesh generating and capping
void ULimbExtractorTool::ExtractBoneToStaticMesh(USkeletalMesh* SkeletalMesh, FName BoneName)
{
#pragma region Create Skel Mesh

	if (!SkeletalMesh || BoneName.IsNone()) return;

	// Step 1: Get vertices
	TSet<int32> SelectedVertices;

	// Step 2: Build Mesh Data
	TArray<FVector3f> FinalVertices;
	TArray<FVector> FinalNormals;
	TArray<FVector2D> FinalUVs;
	TArray<FColor> FinalColors;
	TArray<FVector> FinalTangents;
	TArray<int32> FinalVertexIndices;
	TMap<int32, int32> OldToNewVertexMap; 
	TArray<int32> MaterialIndices;
	TArray<int32> FinalTriangles;
	TArray <FSkinWeightInfo> Skin;
	TArray<FIntVector> IntVecTriangles;
	TSet<FName> ExclBones = TSet(ExcludedBoneNames);
	TArray<int32> Sync;

	if (CreateSkelMesh)
	{

		TArray<int32> OutIndicies;
		TMap<int32, int32> BoneReMap;
		// gets the bone indices associated with the Desired Bone Names children
		GetBoneHierachy(SkeletalMesh, BoneName, ExclBones,  OutIndicies);
		
		// Extract the whole bone chain, or a selection of it if specified
		for (int Index : OutIndicies)
		{
			UE_LOG(LogTemp, Warning, TEXT("Bone Name is - %s"), *SkeletalMesh->GetRefSkeleton().GetBoneName(Index).ToString());
			FName DesiredBone = SkeletalMesh->GetRefSkeleton().GetBoneName(Index);
			ExtractBoneVertices(SkeletalMesh, DesiredBone, FinalVertices, FinalVertexIndices, MinimumWeight /*= 128*/, SelectedVertices);
		}
		
		BuildExtractedMeshData(SkeletalMesh, FinalVertexIndices, FinalVertices, FinalUVs, FinalNormals, FinalColors, FinalTangents, OldToNewVertexMap, IntVecTriangles, Skin, Sync);
		BuildTriangleIndices(SkeletalMesh, SelectedVertices, OldToNewVertexMap, FinalTriangles, MaterialIndices);

//  Step 3: Gather all used skin weight bone indices
		TSet<int32> SkinWeightBoneIndicesUsed;
		for (const FSkinWeightInfo& Info : Skin)
		{
			for (int32 j = 0; j < MAX_TOTAL_INFLUENCES; ++j)
			{
				if (Info.InfluenceWeights[j] > 0)
				{
					SkinWeightBoneIndicesUsed.Add(Info.InfluenceBones[j]);
				}
			}
		}

		CreateSkeletalLimbData(SkeletalMesh, OutLimbSkelMesh, OutIndicies, BoneReMap, SkinWeightBoneIndicesUsed);

		ValidateVertexToSkinWeightMapping(SkeletalMesh, FinalVertices, Skin, Sync);
		ValidateInfluenceBoneMapping(SkeletalMesh, BoneReMap, Skin);

		CreateLimbMeshFromExtractedData(SkeletalMesh, OutLimbSkelMesh, FinalVertices, FinalUVs, FinalNormals, FinalColors, FinalTangents, IntVecTriangles, BoneReMap, Skin, Sync);
		SpawnSkelMeshActor();
		ExcludedBoneNames.Empty();
	
		return;
	}
#pragma endregion
#pragma region Static Mesh creation
	// If we are only extracting a static mesh, then this section does the extraction and capping --
		// Extract the bone vertices from the mesh associated with the bone its skinned to ***** add loop to include bonechains  ***********
		ExtractBoneVertices(SkeletalMesh, BoneName, FinalVertices, FinalVertexIndices, MinimumWeight /*= 128*/, SelectedVertices);

	

	//Take our vertices and build the data needed to construct the triangles for the new mesh
	BuildExtractedMeshData(SkeletalMesh, FinalVertexIndices, FinalVertices, FinalUVs, FinalNormals, FinalColors, FinalTangents, OldToNewVertexMap, IntVecTriangles, Skin, Sync);
	// Step 3: Build Triangles     
	BuildTriangleIndices(SkeletalMesh, SelectedVertices, OldToNewVertexMap, FinalTriangles, MaterialIndices);

	


#pragma region Clean the mesh data


	// convert the tangents to FProcMeshTangents for the create mesh section
	TArray<FProcMeshTangent> MeshTangents;
	for (const FVector& Vector : FinalTangents)
	{
		MeshTangents.Add(FProcMeshTangent(Vector.X, Vector.Y, Vector.Z));
	}

	// Convert the TArray<int32> to TArray<FIntVector for the cleaner function 
	TArray<FIntVector> TempTriangles;

	TempTriangles.Reset(); // Clear any existing data

	const int32 FinTris = FinalTriangles.Num() / 3;
	for (int32 i = 0; i < FinTris; ++i)
	{
		int32 Index0 = FinalTriangles[i * 3];
		int32 Index1 = FinalTriangles[i * 3 + 1];
		int32 Index2 = FinalTriangles[i * 3 + 2];

		TempTriangles.Add(FIntVector(Index0, Index1, Index2));
	}
	//TempTriangles = IntVecTriangles;
	UE_LOG(LogTemp, Warning, TEXT("FinV = %d, FinN = %d, FinUV = %d, FC = %d, MT = %d, FVI = %d, OTNVM = %d, FT = %d"), FinalVertices.Num(), FinalNormals.Num(), FinalUVs.Num(), FinalColors.Num(), MeshTangents.Num(), FinalVertexIndices.Num(), OldToNewVertexMap.Num(), TempTriangles.Num());
	//return;
	
	// Run the cleaner to make sure our mesh is correct for edge capping
	UMeshCleanUtils::UtilCleanMeshForCapping(FinalVertices, TempTriangles, FinalUVs, FinalNormals, .01, 10, DoDiagnostic);

	// Convert TempTriangles back to TArray<int32> for the following functions
	FinalTriangles.Empty();
	for (const FIntVector& Vec : TempTriangles)
	{
		FinalTriangles.Add(Vec.X);
		FinalTriangles.Add(Vec.Y);
		FinalTriangles.Add(Vec.Z);
	}

#pragma endregion

	

#pragma region Material Mapping
	// add a material mapping to each triangle
	TMap<int32, TArray<int32>> MaterialToTriangles;
	for (int32 TriangleIdx = 0; TriangleIdx < MaterialIndices.Num(); ++TriangleIdx)
	{
		if (TriangleIdx * 3 + 2 >= FinalTriangles.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("TriangleIdx %d out of bounds for FinalTriangles (%d)"), TriangleIdx, FinalTriangles.Num());
			continue;
		}
		int32 MaterialIndex = MaterialIndices[TriangleIdx];

		int32 I0 = FinalTriangles[TriangleIdx * 3];
		int32 I1 = FinalTriangles[TriangleIdx * 3 + 1];
		int32 I2 = FinalTriangles[TriangleIdx * 3 + 2];

		// using the & means the next add.(I) directly adds to materialToTriangles
		TArray<int32>& TriList = MaterialToTriangles.FindOrAdd(MaterialIndex);

		TriList.Add(I0);
		TriList.Add(I1);
		TriList.Add(I2);
		
	}
#pragma endregion		

	// Do a check before trying to generate a mesh to make sure we have the required data.. ie arrays with values
	if (FinalVertices.Num() == 0 || FinalNormals.Num() == 0 || FinalUVs.Num() == 0 || FinalColors.Num() == 0 || MeshTangents.Num() == 0 || FinalVertexIndices.Num() == 0 || OldToNewVertexMap.Num() == 0 || FinalTriangles.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("FinV = %d, FinN = %d, FinUV = %d, FC = %d, MT = %d, FVI = %d, OTNVM = %d, FT = %d"), FinalVertices.Num(), FinalNormals.Num(), FinalUVs.Num(), FinalColors.Num(), MeshTangents.Num(), FinalVertexIndices.Num(), OldToNewVertexMap.Num(), FinalTriangles.Num());
		return;
	}

	
#pragma region Spawn actor and create limb mesh
	
	// spawn the actor that will hold the preview mesh, as we need one to have a trnasform and a reference
	SpawnProcMeshActor();

	// now create the actual limb mesh and attach it to the actor
	const TArray<FSkeletalMaterial>& SkeletalMaterials = SkeletalMesh->GetMaterials();
	int32 SectionIndex = 0;

	for (const TPair<int32, TArray<int32>>& Pair : MaterialToTriangles)
	{
		
		int32 MaterialIndex = Pair.Key;
		const TArray<int32>& SectionTriangles = Pair.Value;
		
		AddMaterialSectionToProceduralMesh(
			PreviewMesh,
			SectionIndex,
			MaterialIndex,
			SectionTriangles,
			TArray<FVector>(FinalVertices),
			FinalNormals,
			FinalUVs,
			TArray<FLinearColor>(FinalColors),
			MeshTangents,
			SkeletalMaterials, false
		);

		++SectionIndex;
	}
#pragma endregion
	
	if (AddCap) // BP isnt changing it for some reason
	{
#pragma region Construct cap data
		UE_LOG(LogTemp, Warning, TEXT("Running cap mesh section"));

		// Now we try to find the open edges of the mesh we extracted and sort the edges into loops
		// Then we try to generate a cap out of triangles to attach to the mesh

		// Find open edges and group into loops
		TSet<TPair<int32, int32>> OpenEdges = FindOpenEdgesFromTriangles(FinalTriangles);

		TArray<TArray<int32>> EdgeLoops = SortOpenEdgesIntoLoops(OpenEdges, true);

		// this is just to make sure we have loops with enough vertices
		TArray<TArray<int32>> CleanedLoops;

		for (const TArray<int32>& Loop : EdgeLoops)
		{
			if (Loop.Num() != 0)  // skip only broken loops
			{
				CleanedLoops.Add(Loop);
			}
			//UE_LOG(LogTemp, Warning, TEXT("Raw Detected Loop with %d vertices"), Loop.Num());
		}

		EdgeLoops = CleanedLoops;

		////////////////////////////////////////////////////////////////////////////////////////////
		// Create cap vertex data and merge it with 
		TArray<FVector3f> CapVertices;
		TArray<int32> CapTriangles;
		TArray<FVector> CapNormals;
		TArray<FVector2D> CapUVs;
		TArray<TArray<FVector3f>> TempEdgeLoops;
		TempEdgeLoops.Reserve(EdgeLoops.Num());

		for (const TArray<int32>& Loop : EdgeLoops)
		{
			TArray<FVector3f> LoopPositions;
			LoopPositions.Reserve(Loop.Num());

			for (int32 Index : Loop)
			{
				if (FinalVertices.IsValidIndex(Index))
				{
					LoopPositions.Add(FinalVertices[Index]);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Invalid loop vertex index: %d (FinalVertices.Num() = %d)"), Index, FinalVertices.Num());
				}
			}

			TempEdgeLoops.Add(LoopPositions);
		}

		// finally generate the vertices data we want to give to create mesh section
		if (AddDome)
		{
			for (const TArray<FVector3f>& Loop : TempEdgeLoops)
			{
				GenerateDomedCapFromLoop(Loop, CapVertices, CapTriangles, CapUVs, CapNormals, BP_DomeHieght, BP_NoiseAmount, BP_RingCount, BP_AddNoise);

			}
		}
		else  
		{
			for (const TArray<int32>& Loop : EdgeLoops)
			{

				GenerateCapFanFromLoop(Loop, FinalVertices, CapVertices, CapTriangles);
			}

		}


		if (ShowLoopDebug)
		{
			DrawEdgeLoopsDebug(EdgeLoops, FinalVertices, GetWorld(), PreviewMesh->GetComponentTransform(), 20, .4);
		}
#pragma endregion

#pragma region fill CapBuffers and generate uv data

		// Prepare buffers
		TArray<FVector> CapVerts;

		TArray<FLinearColor> CapColors;
		TArray<FProcMeshTangent> CapTangents;

		for (int32 i = 0; i < CapVertices.Num(); ++i)
		{
			CapVerts.Add(FVector(CapVertices[i]));
			CapNormals.Add(FVector(1, 0, 0)); // Placeholder normal
			//CapUVs.Add(FVector2D(0.5f, 0.5f));
			CapColors.Add(FLinearColor::Red);
			CapTangents.Add(FProcMeshTangent(1, 0, 0));
		}

#pragma endregion

		// WE MUST HAVE FILLED ARRAYS FROM PREVIOUS CODE FOR CAPVERTS, CAPTRIANGLES, and the others
# pragma region create capmesh
	// --- CREATE THE MESH ---
		UProceduralMeshComponent* CapMesh = NewObject<UProceduralMeshComponent>(OwnerActor);

		CapMesh->RegisterComponentWithWorld(GetWorld());
		CapMesh->AttachToComponent(PreviewMesh, FAttachmentTransformRules::KeepRelativeTransform);
		CapMesh->SetWorldLocation(PreviewMesh->GetComponentLocation());
		CapMesh->bUseComplexAsSimpleCollision = true;
		CapMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CapMesh->SetVisibility(true);

		// Create section
		CapMesh->CreateMeshSection_LinearColor(
			0,
			CapVerts,
			CapTriangles,
			CapNormals,
			CapUVs,
			CapColors,
			CapTangents,
			true
		);

		// Apply material
		if (BP_CapMaterial)
		{
			UMaterialInterface* mat = BP_CapMaterial.Get();
			if (mat->IsValidLowLevel())
			{
				CapMesh->SetMaterial(0, mat);

			}
		}
		
#pragma endregion
		// all done
	}

#pragma endregion
}


}

void ULimbExtractorTool::OnEditorUtilityClosed()
{
	if (PreviewMesh)
	{
		PreviewMesh->DestroyComponent();
		OwnerActor->Destroy();
	}
}

// spawns the procedural mesh and sets the PreviewActor member
void ULimbExtractorTool::SpawnProcMeshActor()
{
	if (OwnerActor && OwnerActor->IsValidLowLevel())
	{
		OwnerActor->Destroy();
		OwnerActor = nullptr;
	}
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		static int32 PreviewActorCounter = 0;
		SpawnParams.Name = FName(*FString::Printf(TEXT("LimbPreviewActor_%d"), PreviewActorCounter++));		SpawnParams.ObjectFlags |= RF_Transient;
		OwnerActor = World->SpawnActor<AActor>(AActor::StaticClass(), SpawnParams);
	}
	PreviewMesh = NewObject<UProceduralMeshComponent>(OwnerActor);
	PreviewMesh->RegisterComponent();
	PreviewMesh->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

void ULimbExtractorTool::SpawnSkelMeshActor()
{
	if (OwnerActor && OwnerActor->IsValidLowLevel())
	{
		OwnerActor->Destroy();
		OwnerActor = nullptr;
	}
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		static int32 PreviewActorCounter = 0;
		SpawnParams.Name = FName(*FString::Printf(TEXT("LimbPreviewActor_%d"), PreviewActorCounter++));		SpawnParams.ObjectFlags |= RF_Transient;
		OwnerActor = World->SpawnActor<AActor>(AActor::StaticClass(), SpawnParams);
	}
	USkeletalMeshComponent* SkelMesh = NewObject<USkeletalMeshComponent>(OwnerActor);
	if (OutLimbSkelMesh)
	{
		SkelMesh->SetSkeletalMesh(OutLimbSkelMesh);
		SkelMesh->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		// Register and attach properly
		SkelMesh->RegisterComponent();
		OwnerActor->AddInstanceComponent(SkelMesh);

		OwnerActor->SetRootComponent(SkelMesh); // important!
		// Force material if needed
		if (OutLimbSkelMesh->GetMaterials().Num() == 0)
		{
			UMaterialInterface* DefaultMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
			OutLimbSkelMesh->GetMaterials().Add(FSkeletalMaterial(DefaultMat));
		}
		FString s = SkelMesh->GetBoneLocation(SkelMesh->GetBoneName(0), EBoneSpaces::WorldSpace).ToString() + "  " + SkelMesh->GetBoneName(0).ToString();
		UE_LOG(LogTemp, Warning, TEXT("SkelMesh Set and Attached completed - %s"),*s );
	}
}

// extracts the vertices associated with the selected bone, depending on skinweight
void ULimbExtractorTool::ExtractBoneVertices(USkeletalMesh* SkeletalMesh, FName BoneName, TArray<FVector3f>& OutVertices, TArray<int32>& OutVertexIndices, uint8 MinimumWeightThreshold /*= 128*/, TSet<int32>& SelectedVerts)
{
	if (!SkeletalMesh || BoneName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("No skeletal mesh or bone name provided."));
		return;
	}

	const FSkeletalMeshLODRenderData& LODData = SkeletalMesh->GetResourceForRendering()->LODRenderData[0];
	const FStaticMeshVertexBuffers& VertexBuffers = LODData.StaticVertexBuffers;
	const FSkinWeightVertexBuffer& SkinWeights = *LODData.GetSkinWeightVertexBuffer();
	const FPositionVertexBuffer& PositionBuffer = VertexBuffers.PositionVertexBuffer;
	
		int32 TargetBoneIndex = SkeletalMesh->GetRefSkeleton().FindBoneIndex(BoneName);
		if (TargetBoneIndex == INDEX_NONE)
		{
			UE_LOG(LogTemp, Error, TEXT("Bone %s not found."), *BoneName.ToString());
			return;
		}

		int32 VertexBase = 0; // Tracks offset across sections

		for (int32 SectionIndex = 0; SectionIndex < LODData.RenderSections.Num(); ++SectionIndex)
		{
			const FSkelMeshRenderSection& Section = LODData.RenderSections[SectionIndex];
			const TArray<FBoneIndexType>& BoneMap = Section.BoneMap;
			const int32 NumVertices = Section.NumVertices;

			for (int32 i = 0; i < NumVertices; ++i)
			{
				int32 VertexIndex = VertexBase + i;
				FSkinWeightInfo SkinWeight = SkinWeights.GetVertexSkinWeights(VertexIndex);

				for (int32 InfluenceIndex = 0; InfluenceIndex < MAX_TOTAL_INFLUENCES; ++InfluenceIndex)
				{
					const uint8 CompactBoneIndex = SkinWeight.InfluenceBones[InfluenceIndex];
					const uint8 Weight = SkinWeight.InfluenceWeights[InfluenceIndex];

					if (Weight == 0)
						continue;

					if (!BoneMap.IsValidIndex(CompactBoneIndex))
					{
						UE_LOG(LogTemp, Warning, TEXT("Invalid compact index %d at section %d"), CompactBoneIndex, SectionIndex);
						continue;
					}

					const int32 SkeletonBoneIndex = BoneMap[CompactBoneIndex];

					if (SkeletonBoneIndex == TargetBoneIndex && Weight >= MinimumWeightThreshold)
					{
						OutVertices.Add(PositionBuffer.VertexPosition(VertexIndex));
						OutVertexIndices.Add(VertexIndex);
						SelectedVerts.Add(VertexIndex);
						break;
					}
				}
			}

			VertexBase += NumVertices; // Advance to next section's base
		}
	
	
	

	UE_LOG(LogTemp, Log, TEXT("Extracted %d vertices for bone '%s' (index %d)"), OutVertices.Num(), *BoneName.ToString(), TargetBoneIndex);

}

void ULimbExtractorTool::GetBoneHierachy(
	USkeletalMesh* SkeletalMesh,
	FName RootBone,
	const TSet<FName>& ExcludedBones,
	TArray<int32>& OutSortedBoneIndices)
{
	OutSortedBoneIndices.Empty();

	if (!SkeletalMesh || RootBone.IsNone()) return;

	const FReferenceSkeleton& RefSkeleton = SkeletalMesh->GetRefSkeleton();
	const int32 RootIndex = RefSkeleton.FindBoneIndex(RootBone);
	if (RootIndex == INDEX_NONE) return;

	TSet<int32> Visited;

	// Recursive lambda to collect bone + its children
	TFunction<void(int32)> Visit = [&](int32 BoneIndex)
		{
			if (!RefSkeleton.IsValidIndex(BoneIndex) || Visited.Contains(BoneIndex))
				return;

			FName BoneName = RefSkeleton.GetBoneName(BoneIndex);
			if (ExcludedBones.Contains(BoneName))
				return;

			Visited.Add(BoneIndex);
			OutSortedBoneIndices.Add(BoneIndex);

			// Now visit all children
			for (int32 ChildIndex = 0; ChildIndex < RefSkeleton.GetNum(); ++ChildIndex)
			{
				if (RefSkeleton.GetParentIndex(ChildIndex) == BoneIndex)
				{
					Visit(ChildIndex);
				}
			}
		};

	Visit(RootIndex);

	UE_LOG(LogTemp, Warning, TEXT("? Hierarchy from root '%s': %d bones"), *RootBone.ToString(), OutSortedBoneIndices.Num());
}

// Creates the bone chain and injects it into the new Limb mesh skeleton
void ULimbExtractorTool::CreateSkeletalLimbData(
	USkeletalMesh* SkeletalMesh,
	USkeletalMesh*& OutLimbMesh,
	const TArray<int32>& SortedBoneIndices,
	TMap<int32, int32>& OutBoneRemap, const TSet<int32>& SkinWeightBoneIndicesUsed)
{
	/*if (!SkeletalMesh || SortedBoneIndices.Num() == 0) return;

	OutLimbMesh = NewObject<USkeletalMesh>(GetTransientPackage(), NAME_None, RF_Public | RF_Standalone);
	OutLimbMesh->SetSkeleton(SkeletalMesh->GetSkeleton());*/

	//const FReferenceSkeleton& RefSkeleton = SkeletalMesh->GetRefSkeleton();
	//FReferenceSkeletonModifier RefSkelModifier(OutLimbMesh->GetRefSkeleton(), SkeletalMesh->GetSkeleton());

	//TArray<FMeshBoneInfo> FinalBoneInfo;
	//TArray<FTransform> FinalBonePose;
	//TMap<int32, int32> OldToNewIndex;

	//for (int32 OldIndex : SortedBoneIndices)
	//{
	//	if (!RefSkeleton.IsValidIndex(OldIndex)) continue;

	//	FMeshBoneInfo BoneInfo = RefSkeleton.GetRefBoneInfo()[OldIndex];
	//	FTransform BoneTransform = RefSkeleton.GetRefBonePose()[OldIndex];
	//	
	//	int32 ParentIndex = BoneInfo.ParentIndex;
	//	if (OldToNewIndex.Contains(ParentIndex))
	//	{
	//		BoneInfo.ParentIndex = OldToNewIndex[ParentIndex];
	//	}
	//	else
	//	{
	//		BoneInfo.ParentIndex = INDEX_NONE;
	//	}

	//	int32 NewIndex = FinalBoneInfo.Num();
	//	OldToNewIndex.Add(OldIndex, NewIndex);
	//	OutBoneRemap.Add(OldIndex, NewIndex);

	//	FinalBoneInfo.Add(BoneInfo);
	//	FinalBonePose.Add(BoneTransform);
	//}

	//// Shift root bone to original bind pose location (if available)
	//if (FinalBoneInfo.Num() > 0 && SortedBoneIndices.Num() > 0)
	//{
	//	const int32 OldRootIdx = SortedBoneIndices[0];
	//	if (RefSkeleton.IsValidIndex(OldRootIdx))
	//	{
	//		FinalBonePose[0] = RefSkeleton.GetRefBonePose()[OldRootIdx];
	//	}
	//}

	//for (int32 i = 0; i < FinalBoneInfo.Num(); ++i)
	//{
	//	RefSkelModifier.Add(FinalBoneInfo[i], FinalBonePose[i]);
	//}
	

	// Required in 5.0.3
	//OutLimbMesh->GetRefSkeleton().RebuildRefSkeleton(SkeletalMesh->GetSkeleton(), true);

	//UE_LOG(LogTemp, Warning, TEXT("? Rebuilt skeleton with %d bones"), FinalBoneInfo.Num());

	// After RefSkelModifier.Add() loop:
	//int32 NewRootIndex = 0; // usually first bone in sorted array
	//int32 OldRootIndex = SortedBoneIndices[0];
	//FTransform Accumulated = GetAccumulatedTransform(SkeletalMesh->GetRefSkeleton(), OldRootIndex);
	
	//RefSkelModifier.UpdateRefPoseTransform(NewRootIndex, Accumulated);
	if (!SkeletalMesh || SortedBoneIndices.Num() == 0) return;

	const FReferenceSkeleton& RefSkeleton = SkeletalMesh->GetRefSkeleton();

	OutLimbMesh = NewObject<USkeletalMesh>(GetTransientPackage(), NAME_None, RF_Public | RF_Standalone);
	OutLimbMesh->SetSkeleton(SkeletalMesh->GetSkeleton());

	FReferenceSkeletonModifier Modifier(OutLimbMesh->GetRefSkeleton(), SkeletalMesh->GetSkeleton());

	// ? FIX 1: Only use bones that actually have skin weights
	// Don't blindly combine all hierarchy bones - filter to only needed ones
	TSet<int32> ActuallyUsedBones;

	// Add bones that have skin weights
	for (int32 BoneIdx : SkinWeightBoneIndicesUsed)
	{
		ActuallyUsedBones.Add(BoneIdx);
	}

	// ? FIX 2: Add parent chain ONLY for bones that are actually used
	TSet<int32> ParentChainBones;
	for (int32 UsedBone : ActuallyUsedBones)
	{
		int32 ParentIdx = RefSkeleton.GetParentIndex(UsedBone);
		while (RefSkeleton.IsValidIndex(ParentIdx))
		{
			ParentChainBones.Add(ParentIdx);
			ParentIdx = RefSkeleton.GetParentIndex(ParentIdx);
		}
	}

	// Combine actually used bones with their required parent chain
	TSet<int32> FinalBoneSet = ActuallyUsedBones;
	FinalBoneSet.Append(ParentChainBones);

	// ? FIX 3: Only add SortedBoneIndices if they're in the final set
	// This prevents adding unnecessary bones from the hierarchy
	for (int32 BoneIdx : SortedBoneIndices)
	{
		if (FinalBoneSet.Contains(BoneIdx))
		{
			FinalBoneSet.Add(BoneIdx);
		}
	}

	TArray<int32> Sorted = FinalBoneSet.Array();
	TopologicallySortBones(RefSkeleton, FinalBoneSet, Sorted);

	TMap<int32, int32> OldToNewIndex;

	for (int32 OldIndex : Sorted)
	{
		if (!RefSkeleton.IsValidIndex(OldIndex)) continue;

		FMeshBoneInfo BoneInfo = RefSkeleton.GetRefBoneInfo()[OldIndex];
		FTransform BoneTransform = RefSkeleton.GetRefBonePose()[OldIndex];

		int32 ParentIndex = BoneInfo.ParentIndex;
		if (OldToNewIndex.Contains(ParentIndex))
		{
			BoneInfo.ParentIndex = OldToNewIndex[ParentIndex];
		}
		else
		{
			BoneInfo.ParentIndex = INDEX_NONE;
		}

		const int32 NewIndex = OldToNewIndex.Num();
		OldToNewIndex.Add(OldIndex, NewIndex);
		OutBoneRemap.Add(OldIndex, NewIndex);

		Modifier.Add(BoneInfo, BoneTransform);
	}

	OutLimbMesh->GetRefSkeleton().RebuildRefSkeleton(SkeletalMesh->GetSkeleton(), true);

	if (Sorted.Num() > 0)
	{
		int32 OldRoot = Sorted[0];
		int32 NewRoot = OldToNewIndex[OldRoot];
		FTransform WorldBindPose = GetAccumulatedTransform(SkeletalMesh->GetRefSkeleton(), OldRoot);
		Modifier.UpdateRefPoseTransform(NewRoot, WorldBindPose);
	}
}

// Used in the CreateSkeletalLimbData function for accumulating the parent bone bindpose transforms and ensuring the new root bone has the correct mesh position
FTransform ULimbExtractorTool::GetAccumulatedTransform(const FReferenceSkeleton& RefSkeleton, int32 BoneIndex)
{
	FTransform Accumulated = FTransform::Identity;
	int32 i = RefSkeleton.GetParentIndex(BoneIndex);
	
	while (RefSkeleton.IsValidIndex(BoneIndex))
	{
		FString s = RefSkeleton.GetBoneName(BoneIndex).ToString();
		UE_LOG(LogTemp, Warning, TEXT("Accumulating from - %s"), *s);
		Accumulated *= RefSkeleton.GetRefBonePose()[BoneIndex];// *= , not transform * accumulated;
		BoneIndex = RefSkeleton.GetParentIndex(BoneIndex);
		

	}
	return Accumulated;
}

// Takes all the collected info and creates the new skeletal mesh and skins it, then calls SpawnSkelMeshActor 
void ULimbExtractorTool::CreateLimbMeshFromExtractedData(
	USkeletalMesh* SourceMesh,
	USkeletalMesh*& OutLimbMesh,
	const TArray<FVector3f>& Vertices,
	const TArray<FVector2D>& UVs,
	const TArray<FVector>& Normals,
	const TArray<FColor>& Colors,
	const TArray<FVector>& Tangents,
	const TArray<FIntVector>& Triangles,
	const TMap<int32, int32>& BoneRemap,
	const TArray<FSkinWeightInfo>& SkinWeights,
	TArray<int32>& OriginalVertexIndices)
{
#if WITH_EDITORONLY_DATA
	if (!SourceMesh || Vertices.Num() == 0 || Triangles.Num() == 0 || BoneRemap.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("? Invalid mesh input"));
		return;
	}

	FSkeletalMeshModel* MeshModel = OutLimbMesh->GetImportedModel();
	MeshModel->LODModels.Empty();
	FSkeletalMeshLODModel* LODModel = new FSkeletalMeshLODModel();
	MeshModel->LODModels.Add(LODModel);

	FSkelMeshSection Section;
	Section.MaterialIndex = 0;
	Section.BaseVertexIndex = 0;
	Section.NumVertices = Vertices.Num();
	Section.bUse16BitBoneIndex = true;
	Section.SoftVertices.SetNum(Vertices.Num());

	// ? FIX 5: Build BoneMap more efficiently
	// Only add bones that actually have influences
	TSet<int32> BonesWithInfluences;

	// First pass: collect which bones actually have influences
	for (int32 i = 0; i < Vertices.Num(); ++i)
	{
		int32 OriginalIndex = OriginalVertexIndices.IsValidIndex(i) ? OriginalVertexIndices[i] : i;

		if (SkinWeights.IsValidIndex(OriginalIndex))
		{
			const FSkinWeightInfo& WeightInfo = SkinWeights[OriginalIndex];

			for (int32 j = 0; j < MAX_TOTAL_INFLUENCES; ++j)
			{
				uint8 OldBone = WeightInfo.InfluenceBones[j];
				uint8 Weight = WeightInfo.InfluenceWeights[j];

				if (Weight > 0 && BoneRemap.Contains(OldBone))
				{
					BonesWithInfluences.Add(BoneRemap[OldBone]);
				}
			}
		}
	}

	// Build compact BoneMap
	Section.BoneMap.Reset();
	for (int32 BoneIdx : BonesWithInfluences)
	{
		Section.BoneMap.AddUnique(BoneIdx);
	}

	// Sort BoneMap for consistency
	Section.BoneMap.Sort();

	// ? FIX 6: Improved skin weight assignment
	for (int32 i = 0; i < Vertices.Num(); ++i)
	{
		FSoftSkinVertex& V = Section.SoftVertices[i];
		V.Position = Vertices[i];
		V.TangentZ = FVector3f(Normals.IsValidIndex(i) ? Normals[i] : FVector::UpVector);
		V.TangentX = FVector3f(Tangents.IsValidIndex(i) ? Tangents[i] : FVector::ForwardVector);
		V.TangentY = FVector3f(FVector::CrossProduct(FVector(V.TangentZ), FVector(V.TangentX)));
		V.Color = Colors.IsValidIndex(i) ? Colors[i] : FColor::White;
		V.UVs[0] = FVector2f(UVs.IsValidIndex(i) ? UVs[i] : FVector2D::ZeroVector);

		// Initialize all influences to 0
		for (int32 j = 0; j < MAX_TOTAL_INFLUENCES; ++j)
		{
			V.InfluenceBones[j] = 0;
			V.InfluenceWeights[j] = 0;
		}

		int32 OriginalIndex = OriginalVertexIndices.IsValidIndex(i) ? OriginalVertexIndices[i] : i;
		uint32 TotalWeight = 0;
		int32 ValidInfluences = 0;

		if (SkinWeights.IsValidIndex(OriginalIndex))
		{
			const FSkinWeightInfo& WeightInfo = SkinWeights[OriginalIndex];

			for (int32 j = 0; j < MAX_TOTAL_INFLUENCES && ValidInfluences < MAX_TOTAL_INFLUENCES; ++j)
			{
				uint8 OldBone = WeightInfo.InfluenceBones[j];
				uint8 Weight = WeightInfo.InfluenceWeights[j];

				if (Weight > 0 && BoneRemap.Contains(OldBone))
				{
					int32 NewBoneIndex = BoneRemap[OldBone];
					int32 CompactIndex = Section.BoneMap.Find(NewBoneIndex);

					if (CompactIndex != INDEX_NONE)
					{
						V.InfluenceBones[ValidInfluences] = static_cast<uint8>(CompactIndex);
						V.InfluenceWeights[ValidInfluences] = Weight;
						TotalWeight += Weight;
						ValidInfluences++;
					}
				}
			}
		}

		// ? FIX 7: Better fallback handling
		if (ValidInfluences == 0 || TotalWeight == 0)
		{
			// Assign to first bone in BoneMap (usually root)
			V.InfluenceBones[0] = 0;
			V.InfluenceWeights[0] = 255;
			UE_LOG(LogTemp, Warning, TEXT("?? Vertex %d assigned fallback weight to bone %d"),
				i, Section.BoneMap.IsValidIndex(0) ? Section.BoneMap[0] : 0);
		}
		else if (TotalWeight != 255)
		{
			// Normalize weights to sum to 255
			float Scale = 255.0f / TotalWeight;
			uint32 NewTotal = 0;

			for (int32 j = 0; j < ValidInfluences; ++j)
			{
				uint8 NewWeight = FMath::Clamp(FMath::RoundToInt(V.InfluenceWeights[j] * Scale), 1, 255);
				V.InfluenceWeights[j] = NewWeight;
				NewTotal += NewWeight;
			}

			// Adjust first weight to ensure total is exactly 255
			if (NewTotal != 255 && ValidInfluences > 0)
			{
				int32 Diff = 255 - NewTotal;
				V.InfluenceWeights[0] = FMath::Clamp(V.InfluenceWeights[0] + Diff, 1, 255);
			}
		}
	}

	// Fill triangle data
	Section.BaseIndex = LODModel->IndexBuffer.Num();
	for (const FIntVector& Tri : Triangles)
	{
		LODModel->IndexBuffer.Add(Tri.X);
		LODModel->IndexBuffer.Add(Tri.Y);
		LODModel->IndexBuffer.Add(Tri.Z);
	}
	Section.NumTriangles = Triangles.Num();
	LODModel->Sections.Add(Section);
	LODModel->NumVertices = Vertices.Num();

	// Required bones - only add bones that are actually in the BoneMap
	for (int32 BoneIdx : Section.BoneMap)
	{
		LODModel->RequiredBones.AddUnique(BoneIdx);
		LODModel->ActiveBoneIndices.AddUnique(BoneIdx);
	}

	// Set bounds
	FBox Box(ForceInit);
	for (const FVector3f& V : Vertices)
	{
		Box += FVector(V);
	}
	OutLimbMesh->SetImportedBounds(FBoxSphereBounds(Box));

	// Add LODInfo if missing
	if (OutLimbMesh->GetLODInfoArray().Num() == 0)
	{
		FSkeletalMeshLODInfo LODInfo;
		LODInfo.ScreenSize.Default = 1.0f;
		LODInfo.LODHysteresis = 0.02f;
		LODInfo.ReductionSettings.NumOfTrianglesPercentage = 1.0f;
		LODInfo.ReductionSettings.NumOfVertPercentage = 1.0f;
		LODInfo.ReductionSettings.MaxDeviationPercentage = 0.0f;
		OutLimbMesh->AddLODInfo(LODInfo);
	}

	// Material
	UMaterialInterface* Mat = SourceMesh->Materials.IsValidIndex(0) ?
		SourceMesh->Materials[0].MaterialInterface : nullptr;
	if (Mat)
	{
		OutLimbMesh->GetMaterials().Add(FSkeletalMaterial(Mat));
	}

	MeshModel->LODModels[0].NumTexCoords = 1;

	OutLimbMesh->SetHasVertexColors(true);
	OutLimbMesh->SetEnablePerPolyCollision(false);
	OutLimbMesh->CalculateInvRefMatrices();
	OutLimbMesh->PostEditChange();

#else
	UE_LOG(LogTemp, Error, TEXT("? WITH_EDITORONLY_DATA required"));
#endif
}

void ULimbExtractorTool::ValidateVertexToSkinWeightMapping(
	const USkeletalMesh* SourceMesh,
	const TArray<FVector3f>& OutVertices,
	const TArray<FSkinWeightInfo>& OutSkinWeights,
	const TArray<int32>& OriginalVertexIndices)
{
	const FSkeletalMeshRenderData* RenderData = SourceMesh->GetResourceForRendering();
	if (!RenderData || RenderData->LODRenderData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("? Invalid skeletal render data."));
		return;
	}

	const FSkeletalMeshLODRenderData& LOD = RenderData->LODRenderData[0];
	const FPositionVertexBuffer& Positions = LOD.StaticVertexBuffers.PositionVertexBuffer;
	const FSkinWeightVertexBuffer* SkinBuffer = &LOD.SkinWeightVertexBuffer;

	TArray<FSkinWeightInfo> FullSkinWeights;
	SkinBuffer->GetSkinWeights(FullSkinWeights);

	int32 ErrorCount = 0;

	for (int32 i = 0; i < OutVertices.Num(); ++i)
	{
		if (!OriginalVertexIndices.IsValidIndex(i))
		{
			UE_LOG(LogTemp, Error, TEXT("? Missing original index for vertex %d"), i);
			++ErrorCount;
			continue;
		}

		const int32 OrigIdx = OriginalVertexIndices[i];

		// Validate position match
		const FVector3f& Current = OutVertices[i];
		const FVector3f& Original = Positions.VertexPosition(OrigIdx);

		if (!Current.Equals(Original, 0.01f))
		{
			UE_LOG(LogTemp, Warning, TEXT("?? Position mismatch [Vert %d] ? %s vs %s"),
				i, *Current.ToString(), *Original.ToString());
			++ErrorCount;
		}

		// Validate skin weight match
		if (OutSkinWeights.IsValidIndex(i) && FullSkinWeights.IsValidIndex(OrigIdx))
		{
			const FSkinWeightInfo& SW = OutSkinWeights[i];
			const FSkinWeightInfo& OrigSW = FullSkinWeights[OrigIdx];

			for (int32 j = 0; j < MAX_TOTAL_INFLUENCES; ++j)
			{
				if (SW.InfluenceBones[j] != OrigSW.InfluenceBones[j] ||
					SW.InfluenceWeights[j] != OrigSW.InfluenceWeights[j])
				{
					UE_LOG(LogTemp, Warning, TEXT("?? Mismatch [Vert %d Influence %d] Bone: %d/%d Weight: %d/%d"),
						i, j,
						SW.InfluenceBones[j], OrigSW.InfluenceBones[j],
						SW.InfluenceWeights[j], OrigSW.InfluenceWeights[j]);
					++ErrorCount;
					break;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("? Missing skin weight data for vertex %d"), i);
			++ErrorCount;
		}
	}

	if (ErrorCount == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("? Validation passed  vertex positions and skin weights match original mesh."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("? Validation failed  %d mismatch(es) detected."), ErrorCount);
	}
}

void ULimbExtractorTool::ValidateInfluenceBoneMapping(
	USkeletalMesh* SourceMesh,
	const TMap<int32, int32>& BoneRemap,
	const TArray<FSkinWeightInfo>& SkinWeights)
{
	UE_LOG(LogTemp, Display, TEXT("?? Validating influence bones against BoneRemap..."));

	TSet<int32> UsedBones;

	for (int32 i = 0; i < SkinWeights.Num(); ++i)
	{
		const FSkinWeightInfo& Weights = SkinWeights[i];

		for (int32 j = 0; j < MAX_TOTAL_INFLUENCES; ++j)
		{
			uint8 BoneIndex = Weights.InfluenceBones[j];
			uint8 Weight = Weights.InfluenceWeights[j];

			if (Weight > 0)
			{
				UsedBones.Add(BoneIndex);

				if (!BoneRemap.Contains(BoneIndex))
				{
					UE_LOG(LogTemp, Error, TEXT("? Vertex %d influence %d: Bone %d NOT found in BoneRemap!"), i, j, BoneIndex);
				}
			}
		}
	}

	UE_LOG(LogTemp, Display, TEXT("? Validation complete  %d unique bones used"), UsedBones.Num());
}

void ULimbExtractorTool::ExpandBoneRemapToMatchSkinWeights(
	const USkeletalMesh* SourceMesh,
	TMap<int32, int32>& BoneRemap,
	const TArray<FSkinWeightInfo>& SkinWeights)
{
	const FReferenceSkeleton& RefSkeleton = SourceMesh->GetRefSkeleton();
	TSet<int32> NeededBones;

	for (const FSkinWeightInfo& Weights : SkinWeights)
	{
		for (int32 j = 0; j < MAX_TOTAL_INFLUENCES; ++j)
		{
			if (Weights.InfluenceWeights[j] > 0)
			{
				NeededBones.Add(Weights.InfluenceBones[j]);
			}
		}
	}

	for (int32 BoneIndex : NeededBones)
	{
		if (!BoneRemap.Contains(BoneIndex))
		{
			// Add this bone and its parents recursively
			int32 Current = BoneIndex;
			while (Current != INDEX_NONE && !BoneRemap.Contains(Current))
			{
				FMeshBoneInfo Info = RefSkeleton.GetRefBoneInfo()[Current];
				int32 Parent = RefSkeleton.GetParentIndex(Current);

				int32 NewIndex = BoneRemap.Num();
				BoneRemap.Add(Current, NewIndex);
				Current = Parent;
			}
		}
	}

	UE_LOG(LogTemp, Display, TEXT("? BoneRemap expanded to %d entries to match skin weights."), BoneRemap.Num());
}

void ULimbExtractorTool::TopologicallySortBones(const FReferenceSkeleton& RefSkeleton, const TSet<int32>& BoneIndicesIn, TArray<int32>& OutSorted)
{
	OutSorted.Reset();
	TSet<int32> Visited;
	TFunction<void(int32)> Visit = [&](int32 BoneIdx)
		{
			if (!RefSkeleton.IsValidIndex(BoneIdx) || Visited.Contains(BoneIdx)) return;

			int32 Parent = RefSkeleton.GetParentIndex(BoneIdx);
			if (Parent != INDEX_NONE)
			{
				Visit(Parent);
			}

			Visited.Add(BoneIdx);
			OutSorted.Add(BoneIdx);
		};

	for (int32 Bone : BoneIndicesIn)
	{
		Visit(Bone);
	}
}
void ULimbExtractorTool::CreateSkeletalMesh(USkeletalMesh* Mesh,FName Bone)
{
}

// get our collected mesh data and convert it to useful data to plug into the createmeshsection
void ULimbExtractorTool::BuildExtractedMeshData(USkeletalMesh* SkeletalMesh,
	const TArray<int32>& BoneVertexIndices,
	TArray<FVector3f>& OutVertices,
	TArray<FVector2D>& OutUVs,
	TArray<FVector>& OutNormals,
	TArray<FColor>& OutColors,
	TArray<FVector>& OutTangents,
	TMap<int32, int32>& OutOldToNewVertexMap,
	TArray<FIntVector>& OutTriangles,
	TArray<FSkinWeightInfo>& OutSkinWeights,
	TArray<int32>& OriginalVertexIndices)

{
	
	//if (!SkeletalMesh) return;

	//const FSkeletalMeshLODRenderData& LODData = SkeletalMesh->GetResourceForRendering()->LODRenderData[0];
	//const FStaticMeshVertexBuffers& VertexBuffers = LODData.StaticVertexBuffers;
	//const FPositionVertexBuffer& PositionBuffer = VertexBuffers.PositionVertexBuffer;
	//const FSkinWeightVertexBuffer& SkinWeightsBuffer = *LODData.GetSkinWeightVertexBuffer();

	//const int32 NumVerts = PositionBuffer.GetNumVertices();

	//// Build remapping from old index to new index
	//OutOldToNewVertexMap.Empty();
	//OutVertices.Reserve(BoneVertexIndices.Num());
	//OutUVs.Reserve(BoneVertexIndices.Num());
	//OutNormals.Reserve(BoneVertexIndices.Num());
	//OutTangents.Reserve(BoneVertexIndices.Num());
	//OutColors.Reserve(BoneVertexIndices.Num());
	//OutSkinWeights.Reserve(BoneVertexIndices.Num());

	//int32 NewIndex = 0;
	//for (int32 OldIndex : BoneVertexIndices)
	//{
	//	int32 bufferNum = PositionBuffer.GetNumVertices();
	//	//if (bufferNum > 0 && OldIndex < bufferNum) continue;//IsVertexValid(OldIndex)) continue;

	//	// Position
	//	OutVertices.Add(PositionBuffer.VertexPosition(OldIndex));

	//	// UV
	//	FVector2f UV = VertexBuffers.StaticMeshVertexBuffer.GetVertexUV(OldIndex, 0);
	//	OutUVs.Add(FVector2D(UV.X, UV.Y));

	//	// Normal + Tangents
	//	OutNormals.Add(FVector(VertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(OldIndex)));
	//	OutTangents.Add(FVector(VertexBuffers.StaticMeshVertexBuffer.VertexTangentX(OldIndex)));
	//	// Color
	//	int32 ColorVerts = LODData.StaticVertexBuffers.ColorVertexBuffer.GetNumVertices();
	//	if (ColorVerts > OldIndex)
	//	{
	//		OutColors.Add(LODData.StaticVertexBuffers.ColorVertexBuffer.VertexColor(OldIndex));
	//	}
	//	else
	//	{
	//		OutColors.Add(FColor::White);
	//	}

	//	// Skin weights
	//	FSkinWeightInfo Skin;
	//	Skin = SkinWeightsBuffer.GetVertexSkinWeights(OldIndex); // ? This is supported in UE5.0.3
	//	OutSkinWeights.Add(Skin);

	//	// Record the new remapped index
	//	OutOldToNewVertexMap.Add(OldIndex, NewIndex++);
	//}
	//OutSkinWeights.Reset();
	//OutSkinWeights.Reserve(OutVertices.Num());

	//const FSkinWeightVertexBuffer* SkinWeightBuffer = LODData.GetSkinWeightVertexBuffer();

	//for (int32 i = 0; i < BoneVertexIndices.Num(); ++i)
	//{
	//	int32 OriginalIndex = BoneVertexIndices[i];
	//	int32 num = SkinWeightBuffer->GetNumVertices();

	//	if (num  > OriginalIndex)
	//	{
	//		TArray<FSkinWeightInfo> Info;
	//		SkinWeightBuffer->GetSkinWeights(Info);//(OriginalIndex, Info);
	//		OutSkinWeights.Add(Info[OriginalIndex]);
	//	}
	//	else
	//	{
	//		// Fallback: weight to first bone
	//		FSkinWeightInfo Fallback = {};
	//		Fallback.InfluenceBones[0] = 0;
	//		Fallback.InfluenceWeights[0] = 255;
	//		OutSkinWeights.Add(Fallback);
	//	}
	//}


	//// Copy triangle data and remap indices
	//OutTriangles.Reset();

	//for (const FSkelMeshRenderSection& Section : LODData.RenderSections)
	//{
	//	const uint32 NumTriangles = Section.NumTriangles;
	//	const uint32 BaseIndex = Section.BaseIndex;

	//	for (uint32 TriIdx = 0; TriIdx < NumTriangles; ++TriIdx)
	//	{
	//		uint32 I0 = LODData.MultiSizeIndexContainer.GetIndexBuffer()->Get(BaseIndex + TriIdx * 3 + 0);
	//		uint32 I1 = LODData.MultiSizeIndexContainer.GetIndexBuffer()->Get(BaseIndex + TriIdx * 3 + 1);
	//		uint32 I2 = LODData.MultiSizeIndexContainer.GetIndexBuffer()->Get(BaseIndex + TriIdx * 3 + 2);

	//		if (OutOldToNewVertexMap.Contains(I0) && OutOldToNewVertexMap.Contains(I1) && OutOldToNewVertexMap.Contains(I2))
	//		{
	//			OutTriangles.Add(FIntVector(
	//				OutOldToNewVertexMap[I0],
	//				OutOldToNewVertexMap[I1],
	//				OutOldToNewVertexMap[I2]
	//			));
	//		}
	//	}
	//}
	OutVertices.Reset();
	OutUVs.Reset();
	OutNormals.Reset();
	OutColors.Reset();
	OutTangents.Reset();
	OutOldToNewVertexMap.Reset();
	OutSkinWeights.Reset();

	const FSkeletalMeshRenderData* RenderData = SkeletalMesh->GetResourceForRendering();
	if (!RenderData || RenderData->LODRenderData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("? Invalid render data on source mesh."));
		return;
	}

	const FSkeletalMeshLODRenderData& LODData = RenderData->LODRenderData[0];
	const FStaticMeshVertexBuffers& Buffers = LODData.StaticVertexBuffers;
	const FSkinWeightVertexBuffer* SkinWeightBuffer = &LODData.SkinWeightVertexBuffer;

	// Cache entire skin weight buffer into array
	TArray<FSkinWeightInfo> FullSkinWeights;
	SkinWeightBuffer->GetSkinWeights(FullSkinWeights);

	int32 NewIndexCounter = 0;
	int32 num = Buffers.PositionVertexBuffer.GetNumVertices();
	for (int32 OldIndex : BoneVertexIndices)
	{
		if (OldIndex >= num )
			continue;

		//// Skip any invalid vertex
		//if (!Buffers.PositionVertexBuffer.IsVertexValid(OldIndex))
		//	continue;

		// Save mapping before data push
		const int32 NewIndex = OutVertices.Num(); // <- must match i in CreateLimbMesh
		OutOldToNewVertexMap.Add(OldIndex, NewIndex);

		// Copy vertex attributes
		OutVertices.Add(Buffers.PositionVertexBuffer.VertexPosition(OldIndex));
		OutNormals.Add(FVector(Buffers.StaticMeshVertexBuffer.VertexTangentZ(OldIndex)));
		OutTangents.Add(FVector(Buffers.StaticMeshVertexBuffer.VertexTangentX(OldIndex)));
		OutColors.Add(FColor::White);
		OutUVs.Add(FVector2D(Buffers.StaticMeshVertexBuffer.GetVertexUV(OldIndex, 0)));
		// Store original skin weight index for later
		OriginalVertexIndices.Add(OldIndex);
		// Skin weights MUST be per-vertex accurate
		if (FullSkinWeights.IsValidIndex(OldIndex))
		{
			OutSkinWeights.Add(FullSkinWeights[OldIndex]);
		}
		else
		{
			FSkinWeightInfo Fallback = {};
			Fallback.InfluenceBones[0] = 0;
			Fallback.InfluenceWeights[0] = 255;
			OutSkinWeights.Add(Fallback);
		}
	}
	// Copy triangle data and remap indices
	OutTriangles.Reset();

	for (const FSkelMeshRenderSection& Section : LODData.RenderSections)
	{
		const uint32 NumTriangles = Section.NumTriangles;
		const uint32 BaseIndex = Section.BaseIndex;

		for (uint32 TriIdx = 0; TriIdx < NumTriangles; ++TriIdx)
		{
			uint32 I0 = LODData.MultiSizeIndexContainer.GetIndexBuffer()->Get(BaseIndex + TriIdx * 3 + 0);
			uint32 I1 = LODData.MultiSizeIndexContainer.GetIndexBuffer()->Get(BaseIndex + TriIdx * 3 + 1);
			uint32 I2 = LODData.MultiSizeIndexContainer.GetIndexBuffer()->Get(BaseIndex + TriIdx * 3 + 2);

			if (OutOldToNewVertexMap.Contains(I0) && OutOldToNewVertexMap.Contains(I1) && OutOldToNewVertexMap.Contains(I2))
			{
				OutTriangles.Add(FIntVector(
					OutOldToNewVertexMap[I0],
					OutOldToNewVertexMap[I1],
					OutOldToNewVertexMap[I2]
				));
			}
		}
	}
	
}

// gets triangle indices from skeletal mesh and builds a map
void ULimbExtractorTool::BuildTriangleIndices(
	USkeletalMesh* SkeletalMesh, 
	const TSet<int32>& BoneVertexIndexSet,
	const TMap<int32, int32>& OldToNewVertexMap,
	TArray<int32>& OutFinalTriangles, TArray<int32>& OutMaterialIndices)
{
	if (!SkeletalMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("No SkeletalMesh provided"));
		return;
	}

	const FSkeletalMeshLODRenderData& LODData = SkeletalMesh->GetResourceForRendering()->LODRenderData[0];
	const FRawStaticIndexBuffer16or32Interface* IndexBuffer = LODData.MultiSizeIndexContainer.GetIndexBuffer();

	int32 SectionBaseVertex = 0;

	for (int32 SectionIndex = 0; SectionIndex < LODData.RenderSections.Num(); ++SectionIndex)
	{
		const FSkelMeshRenderSection& Section = LODData.RenderSections[SectionIndex];
		const int32 NumTriangles = Section.NumTriangles;
		const int32 BaseIndex = Section.BaseIndex;

		for (int32 Tri = 0; Tri < NumTriangles; ++Tri)
		{
			const int32 TriIndex = BaseIndex + Tri * 3;

			int32 Index0 = IndexBuffer->Get(TriIndex);
			int32 Index1 = IndexBuffer->Get(TriIndex + 1);
			int32 Index2 = IndexBuffer->Get(TriIndex + 2);

			// Adjust indices to global vertex space
			Index0 += SectionBaseVertex;
			Index1 += SectionBaseVertex;
			Index2 += SectionBaseVertex;

			// Only keep triangle if all its vertices were extracted
			if (BoneVertexIndexSet.Contains(Index0) &&
				BoneVertexIndexSet.Contains(Index1) &&
				BoneVertexIndexSet.Contains(Index2))
			{
				const int32* Remap0 = OldToNewVertexMap.Find(Index0);
				const int32* Remap1 = OldToNewVertexMap.Find(Index1);
				const int32* Remap2 = OldToNewVertexMap.Find(Index2);

				if (Remap0 && Remap1 && Remap2)
				{
					OutFinalTriangles.Add(*Remap0);
					OutFinalTriangles.Add(*Remap1);
					OutFinalTriangles.Add(*Remap2);

					OutMaterialIndices.Add(Section.MaterialIndex); // Store per-triangle material

				}
			}
		}

		SectionBaseVertex += Section.NumVertices;
	}
}

// the function to generate eaech mesh section and add its properties
void ULimbExtractorTool::AddMaterialSectionToProceduralMesh(
	UProceduralMeshComponent* NewMesh,
	int32 SectionIndex,
	int32 MaterialIndex,
	const TArray<int32>& SectionTriangles,
	const TArray<FVector>& FinalVertices,
	const TArray<FVector>& FinalNormals,
	const TArray<FVector2D>& FinalUVs,
	const TArray<FLinearColor>& FinalColors,
	const TArray<FProcMeshTangent>& MeshTangents,
	const TArray<FSkeletalMaterial>& SkeletalMaterials,
	bool bSkipRemap
)
{
	// If we're skipping remap (e.g. cap geometry), we just feed the full data directly
	if (bSkipRemap)
	{
		if (!ensure(FinalVertices.Num() == FinalNormals.Num() &&
			FinalVertices.Num() == FinalUVs.Num() &&
			FinalVertices.Num() == FinalColors.Num() &&
			FinalVertices.Num() == MeshTangents.Num()))
		{
			UE_LOG(LogTemp, Error, TEXT("Mismatch in vertex attribute sizes for cap section."));
			return;
		}

		NewMesh->CreateMeshSection_LinearColor(
			SectionIndex,
			FinalVertices,
			SectionTriangles, // already global, no remap
			FinalNormals,
			FinalUVs,
			FinalColors,
			MeshTangents,
			true // collision
		);

		if (SkeletalMaterials.IsValidIndex(MaterialIndex))
		{
			UMaterialInterface* Material = SkeletalMaterials[MaterialIndex].MaterialInterface;
			if (Material && Material->IsValidLowLevel())
			{
				NewMesh->SetMaterial(SectionIndex, Material);
			}
		}
		return;
	}

	// --------------------------------------------
	// Normal path: remap triangles into local indices
	// --------------------------------------------

	TSet<int32> UniqueVertexIndices;
	for (int32 Index : SectionTriangles)
	{
		UniqueVertexIndices.Add(Index);
	}

	TArray<FVector> SectionVertices;
	TArray<FVector> SectionNormals;
	TArray<FVector2D> SectionUVs;
	TArray<FLinearColor> SectionColors;
	TArray<FProcMeshTangent> SectionTangents;
	TMap<int32, int32> GlobalToLocalIndex;

	int32 LocalVertexCounter = 0;
	for (int32 GlobalIndex : UniqueVertexIndices)
	{
		if (!FinalVertices.IsValidIndex(GlobalIndex) ||
			!FinalNormals.IsValidIndex(GlobalIndex) ||
			!FinalUVs.IsValidIndex(GlobalIndex) ||
			!FinalColors.IsValidIndex(GlobalIndex) ||
			!MeshTangents.IsValidIndex(GlobalIndex))
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid GlobalIndex: %d | V:%d N:%d UV:%d C:%d T:%d"),
				GlobalIndex,
				FinalVertices.Num(),
				FinalNormals.Num(),
				FinalUVs.Num(),
				FinalColors.Num(),
				MeshTangents.Num());
			continue;
		}

		GlobalToLocalIndex.Add(GlobalIndex, LocalVertexCounter++);
		SectionVertices.Add(FinalVertices[GlobalIndex]);
		SectionNormals.Add(FinalNormals[GlobalIndex]);
		SectionUVs.Add(FinalUVs[GlobalIndex]);
		SectionColors.Add(FinalColors[GlobalIndex]);
		SectionTangents.Add(MeshTangents[GlobalIndex]);
	}

	TArray<int32> LocalTriangles;
	for (int32 i = 0; i < SectionTriangles.Num(); i += 3)
	{
		const int32* A = GlobalToLocalIndex.Find(SectionTriangles[i]);
		const int32* B = GlobalToLocalIndex.Find(SectionTriangles[i + 1]);
		const int32* C = GlobalToLocalIndex.Find(SectionTriangles[i + 2]);

		if (A && B && C)
		{
			LocalTriangles.Add(*A);
			LocalTriangles.Add(*B);
			LocalTriangles.Add(*C);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Missing mapping for triangle %d"), i / 3);
		}
	}

	NewMesh->CreateMeshSection_LinearColor(
		SectionIndex,
		SectionVertices,
		LocalTriangles,
		SectionNormals,
		SectionUVs,
		SectionColors,
		SectionTangents,
		true
	);

	if (SkeletalMaterials.IsValidIndex(MaterialIndex))
	{
		UMaterialInterface* Material = SkeletalMaterials[MaterialIndex].MaterialInterface;
		if (Material && Material->IsValidLowLevel())
		{
			NewMesh->SetMaterial(SectionIndex, Material);
		}
	}
}

// Function to group open edges into edge loops
TArray<TArray<int32>> ULimbExtractorTool::SortOpenEdgesIntoLoops(const TSet<TPair<int32, int32>>& OpenEdges, bool bEnforceCCW)
{
	/*TArray<TArray<int32>> Loops;

	// Step 1: Build directed edge map
	TMultiMap<int32, int32> ForwardMap;
	for (const TPair<int32, int32>& Edge : OpenEdges)
	{
		ForwardMap.Add(Edge.Key, Edge.Value);
		ForwardMap.Add(Edge.Value, Edge.Key); // Ensure we can find both directions
	}

	TSet<TPair<int32, int32>> VisitedEdges;

	// Step 2: Traverse unvisited boundary edges
	for (const TPair<int32, int32>& Edge : OpenEdges)
	{
		if (VisitedEdges.Contains(Edge) || VisitedEdges.Contains(TPair<int32, int32>(Edge.Value, Edge.Key)))
		{
			continue;
		}

		TArray<int32> Loop;
		int32 Start = Edge.Key;
		int32 Current = Edge.Value;

		Loop.Add(Start);
		Loop.Add(Current);
		VisitedEdges.Add(Edge);

		while (true)
		{
			// Find the next connected vertex that continues the loop
			TArray<int32> Candidates;
			ForwardMap.MultiFind(Current, Candidates);

			int32 Next = -1;
			for (int32 Candidate : Candidates)
			{
				if (Candidate != Loop[Loop.Num() - 2]) // Don't go back
				{
					TPair<int32, int32> NextEdge(Current, Candidate);
					if (!VisitedEdges.Contains(NextEdge))
					{
						Next = Candidate;
						VisitedEdges.Add(NextEdge);
						break;
					}
				}
			}

			if (Next == -1 || Next == Start)
			{
				break;
			}

			Loop.Add(Next);
			Current = Next;
		}

		// Close the loop if we returned to start
		int32* MappedValue = ForwardMap.Find(Loop.Last());
		if (Loop[0] == Loop.Last() || (MappedValue && *MappedValue == Loop[0]))
		{
			// Loop is closed
			Loops.Add(Loop);
		}
	}

	return Loops;*/
	// 1. Build adjacency map (undirected)
	TMultiMap<int32, int32> Adjacency;
	for (const TPair<int32, int32>& Edge : OpenEdges)
	{
		Adjacency.Add(Edge.Key, Edge.Value);
		Adjacency.Add(Edge.Value, Edge.Key); // bidirectional
	}

	TSet<int32> Visited;
	TArray<TArray<int32>> Loops;

	for (const TPair<int32, int32>& Edge : OpenEdges)
	{
		int32 Start = Edge.Key;
		if (Visited.Contains(Start))
			continue;

		TArray<int32> Loop;
		int32 Current = Start;
		int32 Previous = INDEX_NONE;

		while (true)
		{
			Loop.Add(Current);
			Visited.Add(Current);

			TArray<int32> Neighbors;
			Adjacency.MultiFind(Current, Neighbors);

			// Remove previous to avoid going backwards
			if (Previous != INDEX_NONE)
			{
				Neighbors.Remove(Previous);
			}

			if (Neighbors.Num() == 0)
			{
				break; // dead end
			}

			Previous = Current;
			Current = Neighbors[0]; // continue
			if (Current == Start)
			{
				Loop.Add(Current); // complete the loop
				break;
			}
			else if (Visited.Contains(Current))
			{
				break; // ran into another loop
			}
		}

		if (Loop.Num() >= 3)
		{
			Loops.Add(Loop);
		}
	}

	return Loops;

}

// Finds all the open edges in a mesh, and outputs a Set for the loop-sorter
TSet<TPair<int32, int32>> ULimbExtractorTool::FindOpenEdgesFromTriangles(const TArray<int32> FinTriangles)
{
	TMap<TPair<int32, int32>, int32> EdgeCount;
	TArray<TPair<int32, int32>> DirectedEdges;

	for (int32 i = 0; i < FinTriangles.Num(); i += 3)
	{
		int32 A = FinTriangles[i];
		int32 B = FinTriangles[i + 1];
		int32 C = FinTriangles[i + 2];

		TPair<int32, int32> Edges[3] = {
			TPair<int32, int32>(A, B),
			TPair<int32, int32>(B, C),
			TPair<int32, int32>(C, A),
		};

		for (auto& Edge : Edges)
		{
			TPair<int32, int32> Key = MakeTuple(FMath::Min(Edge.Key, Edge.Value), FMath::Max(Edge.Key, Edge.Value));
			EdgeCount.FindOrAdd(Key)++;
			DirectedEdges.Add(Edge); // store directed for later
		}
	}

	TSet<TPair<int32, int32>> OpenEdges;
	for (const TPair<int32, int32>& Edge : DirectedEdges)
	{
		TPair<int32, int32> Key = MakeTuple(FMath::Min(Edge.Key, Edge.Value), FMath::Max(Edge.Key, Edge.Value));
		if (EdgeCount[Key] == 1)
		{
			OpenEdges.Add(Edge); // preserve direction
		}
	}
	return OpenEdges;
}

// Takes all our final computed data and generates a cap
void ULimbExtractorTool::GenerateCapFanFromLoop(
	const TArray<int32>& Loop,
	const TArray<FVector3f>& FinalVertices,
	TArray<FVector3f>& CapVertices,
	TArray<int32>& CapTriangles)
{
	TArray<FVector3f> LoopPositions;
	for (int32 Index : Loop)
	{
		LoopPositions.Add(FinalVertices[Index]);
	}

	// Compute center in 3D
	FVector3f Center(0);
	for (const FVector3f& V : LoopPositions)
	{
		Center += V;
	}
	Center /= LoopPositions.Num();

	// Newell's method for best-fit normal
	FVector3f Normal(0);
	for (int32 i = 0; i < LoopPositions.Num(); ++i)
	{
		const FVector3f& Current = LoopPositions[i];
		const FVector3f& Next = LoopPositions[(i + 1) % LoopPositions.Num()];
		Normal.X += (Current.Y - Next.Y) * (Current.Z + Next.Z);
		Normal.Y += (Current.Z - Next.Z) * (Current.X + Next.X);
		Normal.Z += (Current.X - Next.X) * (Current.Y + Next.Y);
	}
	Normal = Normal.GetSafeNormal();

	// Basis on plane
	FVector3f AxisX = (LoopPositions[1] - LoopPositions[0]).GetSafeNormal();
	FVector3f AxisY = FVector3f::CrossProduct(Normal, AxisX).GetSafeNormal();

	// Sort points CCW around center
	TArray<TPair<float, int32>> AngledPoints;
	for (int32 i = 0; i < LoopPositions.Num(); ++i)
	{
		FVector3f Offset = LoopPositions[i] - Center;
		float X = FVector3f::DotProduct(Offset, AxisX);
		float Y = FVector3f::DotProduct(Offset, AxisY);
		float Angle = FMath::Atan2(Y, X);
		AngledPoints.Add({ Angle, i });
	}
	AngledPoints.Sort([](const auto& A, const auto& B) { return A.Key < B.Key; });

	int32 CenterIndex = CapVertices.Num();
	CapVertices.Add(Center);

	TArray<int32> SortedIndices;
	for (const auto& Pair : AngledPoints)
	{
		CapVertices.Add(LoopPositions[Pair.Value]);
		SortedIndices.Add(CapVertices.Num() - 1);
	}

	for (int32 i = 0; i < SortedIndices.Num(); ++i)
	{
		int32 A = SortedIndices[i];
		int32 B = SortedIndices[(i + 1) % SortedIndices.Num()];
		CapTriangles.Add(CenterIndex);
		CapTriangles.Add(A);
		CapTriangles.Add(B);
	}
}

void ULimbExtractorTool::GenerateDomedCapFromLoop(
	const TArray<FVector3f>& LoopVerts,
	TArray<FVector3f>& OutVertices,
	TArray<int32>& OutTriangles,
	TArray<FVector2D>& OutUVs,
	TArray<FVector>& OutNormals,
	float DomeHeight,
	float NoiseAmount,
	int32 RingCount,
	bool bApplyNoise
)
{
	
if (LoopVerts.Num() < 3) return;

const int32 BaseIndex = OutVertices.Num();
const int32 LoopSize = LoopVerts.Num();

// Compute center of loop
FVector3f Center(0);
for (const FVector3f& V : LoopVerts) Center += V;
Center /= LoopVerts.Num();

// Estimate loop normal with Newell's method
FVector3f Normal(0);
//for (int32 i = 0; i < LoopVerts.Num(); ++i)
//{
//	const FVector3f& Current = LoopVerts[i];
//	const FVector3f& Next = LoopVerts[(i + 1) % LoopVerts.Num()];
//	Normal.X += (Current.Y - Next.Y) * (Current.Z + Next.Z);
//	Normal.Y += (Current.Z - Next.Z) * (Current.X + Next.X);
//	Normal.Z += (Current.X - Next.X) * (Current.Y + Next.Y);
//}
//Normal = Normal.GetSafeNormal();
// 
// Use the compute normal function, as it removes excess hieght difference vertices from the averaging logic
Normal = ComputeLoopNormalWithTolerance(LoopVerts, 1.5f);  // If loop vertices are significantly further from the rest, then filter them out so the normal is averaged  using the rest

// Store ring indices
TArray<TArray<int32>> RingIndices;

// Push outer ring (loop itself)
TArray<int32> OuterRing;
for (const FVector3f& V : LoopVerts)
{
	OuterRing.Add(OutVertices.Add(V));
	OutUVs.Add(FVector2D(.5, .5));      // placeholder
	OutNormals.Add(FVector(Normal));  // for now, flat shading
}
RingIndices.Add(OuterRing);

// Generate inner rings
for (int32 Ring = 1; Ring < RingCount; ++Ring)
{
	float T = (Ring - .5) / float(RingCount - .5); /*float(Ring) / float(RingCount);*/ // ensure full span
	float Height = FMath::Sin(T * PI * 0.5f) * DomeHeight;

	TArray<int32> RingVertIndices;

	for (int32 i = 0; i < LoopSize; ++i)
	{
		const FVector3f& Outer = LoopVerts[i];
		FVector3f Direction = (Center - Outer);// .GetSafeNormal(); 
		//FVector3f Pos = Outer + Direction * T * 0.8f  + Normal  * Height;
		FVector3f Pos = FMath::Lerp(Outer, Center, T) + Normal * Height;

		if (bApplyNoise)
		{
			float Noise = FMath::FRandRange(-NoiseAmount, NoiseAmount);
			Pos += Normal * Noise;
		}

		RingVertIndices.Add(OutVertices.Add(Pos));
		OutUVs.Add(FVector2D(.5, .5));       // to be generated
		OutNormals.Add(FVector(Normal));   // smooth normal later
	}

	RingIndices.Add(RingVertIndices);
}

// Add center vertex
const FVector3f DomeCenter = Center + Normal * DomeHeight;
const int32 CenterIndex = OutVertices.Add(DomeCenter);
OutUVs.Add(FVector2D(0, 0));
OutNormals.Add(FVector(Normal));

// Triangulate between rings
for (int32 Ring = 0; Ring < RingIndices.Num() - 1; ++Ring)
{
	const TArray<int32>& Outer = RingIndices[Ring];
	const TArray<int32>& Inner = RingIndices[Ring + 1];

	int32 SegmentCount = FMath::Min(Outer.Num(), Inner.Num());

	for (int32 i = 0; i < SegmentCount; ++i)
	{
		int32 A = Outer[i];
		int32 B = Outer[(i + 1) % SegmentCount];
		int32 C = Inner[i];
		int32 D = Inner[(i + 1) % SegmentCount];

		OutTriangles.Add(A); // outer
		OutTriangles.Add(D); // inner
		OutTriangles.Add(C); // inner

		OutTriangles.Add(A); // outer
		OutTriangles.Add(B); // outer
		OutTriangles.Add(D); // inner
	}
}

// Cap with triangles to center point
const TArray<int32>& InnerRing = RingIndices.Last();
for (int32 i = 0; i < InnerRing.Num(); ++i)
{
	int32 A = InnerRing[i];
	int32 B = InnerRing[(i + 1) % InnerRing.Num()];
	OutTriangles.Add(CenterIndex);
	OutTriangles.Add(A);
	OutTriangles.Add(B); // ? not A, B
}
	ProjectPlanarUVsForCap(OutVertices, DomeCenter, Normal , OutUVs, BP_UVScale, BP_UVRotationDegrees, BP_UVOffset);

	//UE_LOG(LogTemp, Warning, TEXT("array vals are - OutTriangles %d, OutVertices %d"), OutTriangles.Num(), OutVertices.Num());
}

FVector3f ULimbExtractorTool::ComputeLoopNormalWithTolerance(const TArray<FVector3f>& LoopVerts,float ToleranceMultiplier)

{
	if (LoopVerts.Num() < 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeLoopNormalWithTolerance: Loop has fewer than 3 vertices."));
		return FVector3f::UpVector;
	}

	// Step 1: Compute center
	FVector3f Center(0);
	for (const FVector3f& V : LoopVerts)
		Center += V;
	Center /= LoopVerts.Num();

	// Step 2: Estimate rough normal using Newells method
	FVector3f RoughNormal(0);
	for (int32 i = 0; i < LoopVerts.Num(); ++i)
	{
		const FVector3f& A = LoopVerts[i];
		const FVector3f& B = LoopVerts[(i + 1) % LoopVerts.Num()];
		RoughNormal.X += (A.Y - B.Y) * (A.Z + B.Z);
		RoughNormal.Y += (A.Z - B.Z) * (A.X + B.X);
		RoughNormal.Z += (A.X - B.X) * (A.Y + B.Y);
	}
	RoughNormal = RoughNormal.GetSafeNormal();

	// Step 3: Measure distances from plane
	TArray<float> Distances;
	Distances.Reserve(LoopVerts.Num());

	for (const FVector3f& V : LoopVerts)
	{
		float Dist = FVector3f::DotProduct(V - Center, RoughNormal);
		Distances.Add(FMath::Abs(Dist));
	}

	// Step 4: Compute average deviation
	float Mean = 0.f;
	for (float D : Distances)
		Mean += D;
	Mean /= Distances.Num();

	float Tolerance = Mean * ToleranceMultiplier;

	// Step 5: Filter points by distance
	TArray<FVector3f> Filtered;
	Filtered.Reserve(LoopVerts.Num());

	for (int32 i = 0; i < LoopVerts.Num(); ++i)
	{
		if (Distances[i] <= Tolerance)
		{
			Filtered.Add(LoopVerts[i]);
		}
	}

	if (Filtered.Num() < 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeLoopNormalWithTolerance: Too few filtered vertices, falling back to rough normal."));
		return RoughNormal;
	}

	// Step 6: Recalculate normal from filtered points
	FVector3f RefinedNormal(0);
	for (int32 i = 0; i < Filtered.Num(); ++i)
	{
		const FVector3f& A = Filtered[i];
		const FVector3f& B = Filtered[(i + 1) % Filtered.Num()];
		RefinedNormal.X += (A.Y - B.Y) * (A.Z + B.Z);
		RefinedNormal.Y += (A.Z - B.Z) * (A.X + B.X);
		RefinedNormal.Z += (A.X - B.X) * (A.Y + B.Y);
	}

	RefinedNormal = RefinedNormal.GetSafeNormal();
	return RefinedNormal;
}



#pragma region logging
//////////////////////////// LOGGING - VISULAIZERS ////////////////////////////////////////////
// Function to spawn a text label in editor mode
void ULimbExtractorTool::SpawnTextLabelInEditor(UWorld* World, const FVector& Position, const FString& Text, const FColor& Color)
{
	if (!World) return;

	// Ensure we are in editor mode
	if (true)
	{
		// Spawn a TextRenderActor
		ATextRenderActor* TextActor = World->SpawnActor<ATextRenderActor>(Position, FRotator::ZeroRotator);
		if (TextActor)
		{
			// Set the text
			UTextRenderComponent* TextA = TextActor->GetTextRender();//->GetTextRender()->SetText(FText::FromString(Text));
			
			TextA->SetText(FText::FromString(Text));
			// Set the text color
			//TextActor->GetTextRender()
				TextA->SetTextRenderColor(Color);

			// Optionally, adjust the size or alignment
			//TextActor->GetTextRender()
			TextA->SetHorizontalAlignment(EHTA_Right);
			//TextActor->GetTextRender()
			TextA->SetWorldSize(1.0f);

			TextActor->SetLifeSpan(10);
		}
	}
}

void ULimbExtractorTool::LogVertexAdjacency(const TArray<FIntVector>& Triangles, int32 VertexIndex)
{
	TSet<int32> AdjacentVerts;

	for (const FIntVector& Tri : Triangles)
	{
		if (Tri.X == VertexIndex || Tri.Y == VertexIndex || Tri.Z == VertexIndex)
		{
			AdjacentVerts.Add(Tri.X);
			AdjacentVerts.Add(Tri.Y);
			AdjacentVerts.Add(Tri.Z);
		}
	}

	AdjacentVerts.Remove(VertexIndex);
	FString Neighbors;
	for (int32 Adj : AdjacentVerts)
	{
		Neighbors += FString::Printf(TEXT("%d "), Adj);
	}
	if (!Neighbors.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Vertex %d has neighbors: %s"), VertexIndex, *Neighbors);
	}
}

// a checker for the skeleton data
void ULimbExtractorTool::LogBoneIndices(USkeletalMesh* SkeletalMesh, bool bAlsoLogSkeleton /*= false*/)
{
	if (!SkeletalMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("LogBoneIndices: SkeletalMesh is NULL!"));
		return;
	}

	const FReferenceSkeleton& MeshRefSkeleton = SkeletalMesh->GetRefSkeleton();
	const int32 NumMeshBones = MeshRefSkeleton.GetNum();

	UE_LOG(LogTemp, Warning, TEXT("===== SkeletalMesh Bone List ====="));
	for (int32 i = 0; i < NumMeshBones; ++i)
	{
		FName BoneName = MeshRefSkeleton.GetBoneName(i);
		UE_LOG(LogTemp, Warning, TEXT("Mesh Bone Index %d : %s"), i, *BoneName.ToString());
	}

	if (bAlsoLogSkeleton && SkeletalMesh->GetSkeleton())
	{
		const FReferenceSkeleton& SkeletonRef = SkeletalMesh->GetSkeleton()->GetReferenceSkeleton();
		const int32 NumSkeletonBones = SkeletonRef.GetNum();

		UE_LOG(LogTemp, Warning, TEXT("===== USkeleton Asset Bone List ====="));
		for (int32 i = 0; i < NumSkeletonBones; ++i)
		{
			FName BoneName = SkeletonRef.GetBoneName(i);
			UE_LOG(LogTemp, Warning, TEXT("Skeleton Bone Index %d : %s"), i, *BoneName.ToString());
		}
	}
}

// debug lines
void ULimbExtractorTool::DrawEdgeLoopsDebug(
	const TArray<TArray<int32>>& EdgeLoops,
	const TArray<FVector3f>& Vertices,
	UWorld* World,
	const FTransform& MeshTransform,
	float Duration,
	float Thickness)
{
	if (!World) return;

	for (int32 LoopIndex = 0; LoopIndex < EdgeLoops.Num(); ++LoopIndex)
	{
		const TArray<int32>& Loop = EdgeLoops[LoopIndex];

		for (int32 i = 0; i < Loop.Num(); ++i)
		{
			int32 CurrentIndex = Loop[i];
			int32 NextIndex = Loop[(i + 1) % Loop.Num()]; // wrap around

			const FVector3f& V0 = Vertices[CurrentIndex];
			const FVector3f& V1 = Vertices[NextIndex];

			const FVector Idx = FVector(V0) + FVector(0, 0, 5);
			FVector WorldV0 = MeshTransform.TransformPosition(FVector(V0));
			FVector WorldV1 = MeshTransform.TransformPosition(FVector(V1));

			FColor LineColor = FColor::MakeRedToGreenColorFromScalar((float)LoopIndex / (float)EdgeLoops.Num());

			DrawDebugLine(World, WorldV0, WorldV1, LineColor, false, Duration, 0, Thickness);
			DrawDebugPoint(World, WorldV0, 6.0f, LineColor, false, Duration);
			FString num = FString::FromInt(CurrentIndex);
			//SpawnTextLabelInEditor(GetWorld(), Idx, num, FColor::Blue);

		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Drawn %d edge loops for debug"), EdgeLoops.Num());
}

#pragma endregion

#pragma region Cap uv generation
///////// Cap UV section //////////////////


void ULimbExtractorTool::ProjectPlanarUVsForCap(
	const TArray<FVector3f>& CapVertices,
	const FVector3f& PlaneOrigin,
	const FVector3f& PlaneNormal,
	TArray<FVector2D>& OutUVs,
	float UVScale = 1.0f,
	float UVRotationDegrees = 0.0f,
	FVector2D UVOffset = FVector2D::ZeroVector)
{
	// Step 1: Build an orthonormal basis for the slicing plane
	FVector3f U = FVector3f::CrossProduct(PlaneNormal, FVector3f::UpVector);
	if (U.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		U = FVector3f::CrossProduct(PlaneNormal, FVector3f::RightVector);
	}
	U.Normalize();

	FVector3f V = FVector3f::CrossProduct(PlaneNormal, U);
	V.Normalize();

	// Step 2: Prepare rotation matrix (2D)
	const float AngleRad = FMath::DegreesToRadians(UVRotationDegrees);
	const float CosA = FMath::Cos(AngleRad);
	const float SinA = FMath::Sin(AngleRad);

	OutUVs.SetNumUninitialized(CapVertices.Num());

	// Step 3: Project each vertex
	for (int32 i = 0; i < CapVertices.Num(); ++i)
	{
		// Offset from plane origin
		const FVector3f Local = CapVertices[i] - PlaneOrigin;

		// Project onto plane axes
		const float UCoord = FVector3f::DotProduct(Local, U);
		const float VCoord = FVector3f::DotProduct(Local, V);

		// Apply rotation and scale
		float RotU = UCoord * CosA - VCoord * SinA;
		float RotV = UCoord * SinA + VCoord * CosA;

		// Final UV with scale and offset
		OutUVs[i] = (FVector2D(RotU, RotV) * UVScale) + UVOffset;
	}
}

#pragma endregion

////////// Some experimental code

#pragma region experimental
// Maybe look into this later as a way of doing it via UE5 api
void ULimbExtractorTool::FindOpenEdgesFromDescription(const FMeshDescription& MeshDescription, TArray<TArray<int32>>& EdgeLoops)
{
	EdgeLoops.Empty();

	TMap<FEdgeID, TArray<int32>> EdgeMap;

	for (const FEdgeID EdgeID : MeshDescription.Edges().GetElementIDs())
	{
		if (MeshDescription.GetNumEdgeConnectedPolygons(EdgeID) == 1) // Open edge
		{
			int32 val = EdgeID;
			//for(int32 i = 0; i < )
			TArray<FVertexID> ConnectedVertices;
			ConnectedVertices = MeshDescription.GetVertexConnectedEdgeIDs(EdgeID);
			UE_LOG(LogTemp, Warning, TEXT("connected vertices = %d"), ConnectedVertices.Num());
			if (ConnectedVertices.Num() == 2)
			{
				EdgeMap.Add(EdgeID, { ConnectedVertices[0].GetValue(), ConnectedVertices[1].GetValue() });
			}
		}
	}

	// Convert EdgeMap into EdgeLoops
	TSet<int32> ProcessedVertices;
	for (const TPair<FEdgeID, TArray<int32>>& EdgePair : EdgeMap)
	{
		TArray<int32> EdgeLoop;
		EdgeLoop.Append(EdgePair.Value);

		ProcessedVertices.Add(EdgePair.Value[0]);
		ProcessedVertices.Add(EdgePair.Value[1]);

		EdgeLoops.Add(EdgeLoop);
	}
}

void ULimbExtractorTool::ConvertProceduralMeshToMeshDescription(UProceduralMeshComponent* ProcMeshComp, FMeshDescription& MeshDescription)
{
	
		if (!ProcMeshComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid ProceduralMeshComponent!"));
			return;
		}

		FStaticMeshAttributes Attributes(MeshDescription);
		Attributes.Register();

		TArray<FVector3f> Vertices;
		TArray<FVector2D> UVs;
		TArray<FVector> Normals;
		TArray<int32> Triangles;

		// Extract mesh data from ProceduralMeshComponent
		for (int32 SectionIndex = 0; SectionIndex < ProcMeshComp->GetNumSections(); ++SectionIndex)
		{
			FProcMeshSection* MeshSection = ProcMeshComp->GetProcMeshSection(SectionIndex);
			if (!MeshSection) continue;

			for (const FProcMeshVertex& Vertex : MeshSection->ProcVertexBuffer)
			{
				Vertices.Add(FVector3f(Vertex.Position));
				UVs.Add(Vertex.UV0);
				Normals.Add(Vertex.Normal);
			}

			Triangles.Append(MeshSection->ProcIndexBuffer);
		}
		
		// Convert extracted data into MeshDescription
		TVertexAttributesRef<FVector3f> VertexPositions = MeshDescription.GetVertexPositions();
		TArray<FVertexID> VertexIDs;

		for (const FVector3f& Vertex : Vertices)
		{
			FVertexID VertexID = MeshDescription.CreateVertex();
			VertexIDs.Add(VertexID);
			VertexPositions[VertexID] = Vertex; // Correct way to set vertex positions
		}
		
		TArray<FVertexInstanceID> VertexInstances;
		TVertexInstanceAttributesRef<FVector4f> VertexInstanceUVs = MeshDescription.VertexInstanceAttributes().GetAttributesRef<FVector4f>(MeshAttribute::VertexInstance::TextureCoordinate);
		TVertexInstanceAttributesRef<FVector3f> VertexInstanceNormals = MeshDescription.VertexInstanceAttributes().GetAttributesRef<FVector3f>(MeshAttribute::VertexInstance::Normal);

		for (int32 i = 0; i < Vertices.Num(); ++i)
		{
			FVertexInstanceID InstanceID = MeshDescription.CreateVertexInstance(VertexIDs[i]);
			VertexInstances.Add(InstanceID);

			// Ensure attributes are correctly set
			if (MeshDescription.VertexInstanceAttributes().HasAttribute(MeshAttribute::VertexInstance::TextureCoordinate))
			{
				if (VertexInstanceUVs.IsValid())
				{
					VertexInstanceUVs[InstanceID] = FVector4f(UVs[i].X, UVs[i].Y, 0.0f, 0.0f);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("VertexInstanceUVs is null!"));
				}
			}

			if (MeshDescription.VertexInstanceAttributes().HasAttribute(MeshAttribute::VertexInstance::Normal))
			{
				VertexInstanceNormals[InstanceID] = FVector3f(Normals[i]);
			}
		}


		for (int32 i = 0; i < Triangles.Num(); i += 3)
		{
			TArray<FVertexInstanceID> TriangleInstances = {
				VertexInstances[Triangles[i]],
				VertexInstances[Triangles[i + 1]],
				VertexInstances[Triangles[i + 2]]
			};

			FPolygonGroupID PolygonGroupID = MeshDescription.CreatePolygonGroup();
			MeshDescription.CreatePolygon(PolygonGroupID, TriangleInstances);
		}
}

// latest using open mesh idea
void ULimbExtractorTool::FindBoundaryLoopsFromTriangles(const TArray<FIntVector>& Triangles, TArray<TArray<int32>>& OutLoops)
{
	// Step 1: Count edges
	TMap<FEdgeKey, int32> EdgeUsage;
	for (const FIntVector& Tri : Triangles)
	{
		int32 Idx[3] = { Tri.X, Tri.Y, Tri.Z };
		for (int32 i = 0; i < 3; ++i)
		{
			FEdgeKey Key(Idx[i], Idx[(i + 1) % 3]);
			EdgeUsage.FindOrAdd(Key) += 1;
		}
	}

	// Step 2: Collect boundary edges (those used only once)
	TMultiMap<int32, int32> VertexToEdge; // v0 -> v1
	TSet<TPair<int32, int32>> BoundaryEdges;
	for (const auto& Elem : EdgeUsage)
	{
		if (Elem.Value == 1)
		{
			BoundaryEdges.Add({ Elem.Key.A, Elem.Key.B });
			VertexToEdge.Add(Elem.Key.A, Elem.Key.B);
		}
	}

	// Step 3: Sort boundary edges into loops
	TSet<TPair<int32, int32>> VisitedEdges;

	while (BoundaryEdges.Num() > 0)
	{
		TArray<int32> Loop;
		TPair<int32, int32> StartEdge = *BoundaryEdges.CreateIterator();
		BoundaryEdges.Remove(StartEdge);
		VisitedEdges.Add(StartEdge);

		int32 Start = StartEdge.Key;
		int32 Current = StartEdge.Value;

		Loop.Add(Start);
		Loop.Add(Current);

		while (Current != Start)
		{
			TArray<int32> NextVertices;
			VertexToEdge.MultiFind(Current, NextVertices);

			bool bFoundNext = false;
			for (int32 Next : NextVertices)
			{
				TPair<int32, int32> CandidateEdge(Current, Next);
				if (!VisitedEdges.Contains(CandidateEdge) && BoundaryEdges.Contains(CandidateEdge))
				{
					VisitedEdges.Add(CandidateEdge);
					BoundaryEdges.Remove(CandidateEdge);
					Loop.Add(Next);
					Current = Next;
					bFoundNext = true;
					break;
				}
			}

			if (!bFoundNext)
			{
				UE_LOG(LogTemp, Warning, TEXT("Loop was not closed properly. May indicate non-manifold or degenerate edges."));
				break;
			}
		}

		if (Loop.Num() >= 3)
		{
			OutLoops.Add(Loop);
			UE_LOG(LogTemp, Warning, TEXT("OutLoop has %d vertices"), Loop.Num());
		}
	}
}

#pragma endregion


