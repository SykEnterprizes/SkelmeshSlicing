
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
//#include "Rendering/SkeletalMeshLODModel.h"
//#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkinWeightVertexBuffer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/Skeleton.h"
#include "Materials/Material.h"
#include "Misc/Guid.h"
//#include "Rendering/SkinWeightVertexBuffer.h"
#include "Algo/Sort.h"
#include "Math/Plane.h"
#include "Materials/MaterialInstanceConstant.h"
#include <Misc/MessageDialog.h>

#include "RHI.h"

/////////////// Helper functions //////////////////////////////////////////////////////
void ULimbExtractorTool::PrintError(FString string)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *string);
}

void ULimbExtractorTool::SetMinimumWeightThreshold(int32 value)
{
	MinimumWeight = value;
}

void ULimbExtractorTool::AddExclusionBones(TArray<FName> Exclusions)
{
	ExcludedBoneNames = Exclusions;
}

void ULimbExtractorTool::ClearExcusionBoneList() 
{
	ExcludedBoneNames.Empty(); 
}

void ULimbExtractorTool::LoggingAndDebugSwitches(bool ShowLoops, bool MeshDiagnostic)
{
	ShowLoopDebug = ShowLoops;
	DoDiagnostic = MeshDiagnostic;
}

//////////////////// Main functions for the extraction process ////////////////////////////////////////////////////////////////////
// The only called function in BP - does all the mesh generating and capping
void ULimbExtractorTool::ExtractBoneToMesh(USkeletalMesh* SkeletalMesh, FName BoneName, bool CreateSkelMesh, bool CreateBoneChain)
{
	// precautionary deletion of actors, so we dont get level actors with the same name caused by calling the CreateSkelMesh, then CreateStaticMesh
	OnEditorUtilityClosed();

	if (CreateSkelMesh)
	{
		CreateSkeletalMesh(SkeletalMesh, BoneName, CreateBoneChain);
		
	}
	else
	{
		CreateStaticMesh(SkeletalMesh, BoneName, CreateBoneChain);
	}
		
}

// Gets all the bones in a skeletal mesh, and fills the BoneHierachy struct with info we can use later 
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

// gets the material from edgeloop vertices and tris
int32 ULimbExtractorTool::InferLoopMaterialFromPositions(const TArray<FVector3f>& LoopPositions,const TArray<int32>& FinalTriangles,const TArray<FVector3f>& FinalVertices,const TArray<int32>& MaterialIndices,float PositionTolerance = 0.001f)


{
	TMap<int32, int32> MaterialVotes;

	for (int32 TriIdx = 0; TriIdx < FinalTriangles.Num(); TriIdx += 3)
	{
		int32 I0 = FinalTriangles[TriIdx];
		int32 I1 = FinalTriangles[TriIdx + 1];
		int32 I2 = FinalTriangles[TriIdx + 2];
		
		if (!FinalVertices.IsValidIndex(I0) || !FinalVertices.IsValidIndex(I1) || !FinalVertices.IsValidIndex(I2))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Skipping triangle due to out-of-bounds vertex index: [%d, %d, %d], VertexCount = %d"), I0, I1, I2, FinalVertices.Num());
			continue;
		}

		FVector3f V0 = FinalVertices[I0];
		FVector3f V1 = FinalVertices[I1];
		FVector3f V2 = FinalVertices[I2];

		auto MatchesLoop = [&](const FVector3f& V)
			{
				for (const FVector3f& L : LoopPositions)
				{
					if (FVector3f::DistSquared(L, V) < PositionTolerance * PositionTolerance)
						return true;
				}
				return false;
			};

		if (MatchesLoop(V0) || MatchesLoop(V1) || MatchesLoop(V2))
		{
			int32 MatIdx = MaterialIndices[TriIdx / 3];
			MaterialVotes.FindOrAdd(MatIdx)++;
		}
	}

	// Return dominant material
	int32 BestMat = 0, MaxVotes = 0;
	for (const auto& Pair : MaterialVotes)
	{
		if (Pair.Value > MaxVotes)
		{
			BestMat = Pair.Key;
			MaxVotes = Pair.Value;
		}
	}
	return BestMat;
}

// when the editor tool is closed, delete the referenced actors from the world
void ULimbExtractorTool::OnEditorUtilityClosed()
{
	EdgeLoopInfos.Empty();
	Wrapper.EdgeLoops.Empty();
	

	if (PreviewMesh) 
	{
		PreviewMesh->DestroyComponent();
		OwnerActor->Destroy();
	}
	if (SkelMesh)
	{
		SkelMesh->DestroyComponent();
		OwnerActor->Destroy();
		OutLimbSkelMesh = nullptr;
	}
}

// spawns the procedural mesh and creates a PreviewActor member
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
		SpawnParams.Name = FName(*FString::Printf(TEXT("LimbProcMeshPreviewActor_%d"), PreviewActorCounter++));		SpawnParams.ObjectFlags |= RF_Transient;
		OwnerActor = World->SpawnActor<AActor>(AActor::StaticClass(), SpawnParams);
	}
	if(OwnerActor)
	{
		PreviewMesh = NewObject<UProceduralMeshComponent>(OwnerActor);
		PreviewMesh->RegisterComponent();
		PreviewMesh->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		if (PreviewMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("PreviewMesh is valid"));

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PreviewMesh is null"));

		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerActor is null"));
	}
	
}

// spawns the skeletal mesh and creates a PreviewActor
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
		SpawnParams.Name = FName(*FString::Printf(TEXT("LimbSkelMeshPreviewActor_%d"), PreviewActorCounter++));		SpawnParams.ObjectFlags |= RF_Transient;
		OwnerActor = World->SpawnActor<AActor>(AActor::StaticClass(), SpawnParams);
	}
	SkelMesh = NewObject<USkeletalMeshComponent>(OwnerActor);
	if (OutLimbSkelMesh && !OutLimbSkelMesh->GetResourceForRendering()->LODRenderData.IsEmpty())
	{
		

		SkelMesh->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		// Register and attach properly
		SkelMesh->RegisterComponent();
		OwnerActor->AddInstanceComponent(SkelMesh);
		SkelMesh->SetComponentTickEnabled(true);
		SkelMesh->TickComponent(0.f, ELevelTick::LEVELTICK_All, nullptr);
		SkelMesh->SetSkeletalMesh(nullptr);              // Clear existing state
		SkelMesh->SetSkeletalMesh(OutLimbSkelMesh, true);    // 'true' forces RequiredBones refresh
		SkelMesh->InitAnim(false);                       // Ensures bone transform chain is updated
		SkelMesh->RefreshBoneTransforms();               // Forces update
		SkelMesh->MarkRenderStateDirty();                // Required to re-render with new mesh
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

//  extracts vertex data from a skeletal mesh to be used to create sections of that mesh
void ULimbExtractorTool::ExtractSkelMeshVertices(USkeletalMesh* SkeletalMesh, FName BoneName, TArray<FRawVertexData>& VertexData, float MinimumWeightThreshold)
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

	const int32 TargetBoneIndex = SkeletalMesh->GetRefSkeleton().FindBoneIndex(BoneName);
	if (TargetBoneIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("Bone %s not found."), *BoneName.ToString());
		return;
	}

	int32 VertexBase = 0;

	for (int32 SectionIndex = 0; SectionIndex < LODData.RenderSections.Num(); ++SectionIndex)
	{
		const FSkelMeshRenderSection& Section = LODData.RenderSections[SectionIndex];
		const TArray<FBoneIndexType>& BoneMap = Section.BoneMap;
		const int32 NumVertices = Section.NumVertices;

		for (int32 i = 0; i < NumVertices; ++i)
		{
			int32 VertexIndex = VertexBase + i;
			const FSkinWeightInfo SkinWeight = SkinWeights.GetVertexSkinWeights(VertexIndex);

			FRawVertexData Data;
			Data.Position = PositionBuffer.VertexPosition(VertexIndex);
			Data.OriginalVertexIndex = VertexIndex;
			Data.RemappedIndex = VertexData.Num(); // running tally

			bool bHasValidInfluence = false;

			for (int32 InfluenceIndex = 0; InfluenceIndex < MAX_TOTAL_INFLUENCES; ++InfluenceIndex)
			{
				const uint8 CompactBoneIndex = SkinWeight.InfluenceBones[InfluenceIndex];
				const uint8 WeightByte = SkinWeight.InfluenceWeights[InfluenceIndex];

				if (WeightByte == 0 || !BoneMap.IsValidIndex(CompactBoneIndex)) continue;

				const int32 SkeletonBoneIndex = BoneMap[CompactBoneIndex];
				const float Weight = static_cast<float>(WeightByte) / 255.0f;

				//if (SkeletonBoneIndex == TargetBoneIndex && Weight >= MinimumWeightThreshold)
				if (SkeletonBoneIndex == TargetBoneIndex && Weight >= EffectiveThreshold)
				{
					bHasValidInfluence = true;
				}
				
				// Store all valid influences
				Data.BoneInfluences.Add(SkeletonBoneIndex);
				Data.BoneWeights.Add(Weight);
			}

			if (bHasValidInfluence)
			{
				VertexData.Add(Data);
			}
			
		}

		VertexBase += NumVertices;
	}

	//UE_LOG(LogTemp, Log, TEXT("Extracted %d vertices for bone '%s' (index %d)"), VertexData.Num(), *BoneName.ToString(), TargetBoneIndex);
}

// a helper for ease of visualizing per-vertex values during debugging 
void ULimbExtractorTool::DumpVertexDataToCSV(const FString& OutputPath, const TArray<FRawVertexData>& VertexData, const USkeletalMesh* SkeletalMesh)
{
	if (!SkeletalMesh || VertexData.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No skeletal mesh or vertex data available to dump."));
		return;
	}

	const FReferenceSkeleton& RefSkeleton = SkeletalMesh->GetRefSkeleton();
	FString CSVOutput = TEXT("VertexIndex,PosX,PosY,PosZ, BoneIndices ,BoneNames ,Weights \n");

	for (int32 i = 0; i < VertexData.Num(); ++i)
	{
		const FRawVertexData& V = VertexData[i];

		FString BoneIndexStr;
		FString BoneNameStr;
		FString WeightStr;

		for (int32 j = 0; j < V.BoneInfluences.Num(); ++j)
		{
			const int32 BoneIndex = V.BoneInfluences[j];
			const float Weight = V.BoneWeights[j];

			BoneIndexStr += FString::Printf(TEXT("%d "), BoneIndex);
			BoneNameStr += RefSkeleton.IsValidIndex(BoneIndex) ? RefSkeleton.GetBoneName(BoneIndex).ToString() + TEXT(" ") : TEXT("INVALID ");
			WeightStr += FString::Printf(TEXT("%.3f "), Weight);
		}

		CSVOutput += FString::Printf(TEXT("%d,%.3f,%.3f,%.3f,\"%s\",\"%s\",\"%s\"\n"),
			V.OriginalVertexIndex,
			V.Position.X, V.Position.Y, V.Position.Z,
			*BoneIndexStr, *BoneNameStr, *WeightStr);
	}

	if (FFileHelper::SaveStringToFile(CSVOutput, *OutputPath))
	{
		UE_LOG(LogTemp, Log, TEXT("Vertex data successfully saved to %s"), *OutputPath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save vertex data to %s"), *OutputPath);
	}
}

//  gets the bone chain hierachy of the given input bone name, and will excluded the baone chains from the iput exclusion array
void ULimbExtractorTool::GetBoneHierachy(USkeletalMesh* SkeletalMesh,FName RootBone,const TSet<FName>& ExcludedBones,TArray<int32>& OutSortedBoneIndices)
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

	//UE_LOG(LogTemp, Warning, TEXT("✅ Hierarchy from root '%s': %d bones"), *RootBone.ToString(), OutSortedBoneIndices.Num());
}

// Creates the bone chain and injects it into the new Limb mesh skeleton
void ULimbExtractorTool::CreateSkeletalLimbData(USkeletalMesh* SkeletalMesh,USkeletalMesh*& OutLimbMesh,const TArray<int32>& SortedBoneIndices,	TMap<int32, int32>& OutBoneRemap, const TSet<int32>& SkinWeightBoneIndicesUsed)
{
	/// this block manages to extract ONLY the desired bonechain given the starting bone *Desired behavior* - 
	/// eg hand_l - gives hand_l as the root bone, and then all its fingers
	if (!SkeletalMesh || SortedBoneIndices.Num() == 0) return;

	OutLimbMesh = NewObject<USkeletalMesh>(GetTransientPackage(), NAME_None, RF_Public | RF_Standalone);
	OutLimbMesh->SetSkeleton(SkeletalMesh->GetSkeleton());

	const FReferenceSkeleton& RefSkeleton = SkeletalMesh->GetRefSkeleton();
	FReferenceSkeletonModifier RefSkelModifier(OutLimbMesh->GetRefSkeleton(), SkeletalMesh->GetSkeleton());

	TArray<FMeshBoneInfo> FinalBoneInfo;
	TArray<FTransform> FinalBonePose;
	TMap<int32, int32> OldToNewIndex;

	for (int32 OldIndex : SortedBoneIndices)
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

		int32 NewIndex = FinalBoneInfo.Num();
		OldToNewIndex.Add(OldIndex, NewIndex);
		OutBoneRemap.Add(OldIndex, NewIndex);

		FinalBoneInfo.Add(BoneInfo);
		FinalBonePose.Add(BoneTransform);
	}

	// Shift root bone to original bind pose location (if available)
	if (FinalBoneInfo.Num() > 0 && SortedBoneIndices.Num() > 0)
	{
		const int32 OldRootIdx = SortedBoneIndices[0];
		if (RefSkeleton.IsValidIndex(OldRootIdx))
		{
			FinalBonePose[0] = RefSkeleton.GetRefBonePose()[OldRootIdx];
		}
	}

	for (int32 i = 0; i < FinalBoneInfo.Num(); ++i)
	{
		RefSkelModifier.Add(FinalBoneInfo[i], FinalBonePose[i]);
	}
	

	// Required in 5.0.3
	OutLimbMesh->GetRefSkeleton().RebuildRefSkeleton(SkeletalMesh->GetSkeleton(), true);
	
	//UE_LOG(LogTemp, Warning, TEXT("✅ Rebuilt skeleton with %d bones"), FinalBoneInfo.Num());

	// After RefSkelModifier.Add() loop:
	int32 NewRootIndex = 0; // usually first bone in sorted array
	int32 OldRootIndex = SortedBoneIndices[0];
	FTransform Accumulated = GetAccumulatedTransform(SkeletalMesh->GetRefSkeleton(), OldRootIndex);
	RefSkelModifier.UpdateRefPoseTransform(NewRootIndex, Accumulated);
#pragma region variation
	///////   This block adds our desired root bone - eg hand_l and all its fingers, 
	/// but also every parent bone of the hand_l back until the root bone.  The pro is that it helps the sync between verts and skin
	/*if (!SkeletalMesh || SortedBoneIndices.Num() == 0) return;

	OutLimbMesh = NewObject<USkeletalMesh>(GetTransientPackage(), NAME_None, RF_Public | RF_Standalone);
	OutLimbMesh->SetSkeleton(SkeletalMesh->GetSkeleton());

	const FReferenceSkeleton& RefSkeleton = SkeletalMesh->GetRefSkeleton();
	FReferenceSkeletonModifier Modifier(OutLimbMesh->GetRefSkeleton(), SkeletalMesh->GetSkeleton());

	// FIXED: Collect ALL bones needed (hierarchy + skin weight referenced)
	TSet<int32> AllNeededBones = TSet<int32>(SortedBoneIndices);

	// Add all skin weight referenced bones
	for (int32 Bone : SkinWeightBoneIndicesUsed)
	{
		AllNeededBones.Add(Bone);

		// CRITICAL: Add parent chain for each skin weight bone
		int32 ParentIndex = RefSkeleton.GetParentIndex(Bone);
		while (ParentIndex != INDEX_NONE)
		{
			AllNeededBones.Add(ParentIndex);
			ParentIndex = RefSkeleton.GetParentIndex(ParentIndex);
		}
	}

	// Sort bones by hierarchy (parents before children)
	TArray<int32> SortedBones = AllNeededBones.Array();
	SortedBones.Sort([&RefSkeleton](const int32& A, const int32& B) {
		// Simple depth-based sort - bones closer to root come first
		auto GetDepth = [&RefSkeleton](int32 BoneIndex) -> int32 {
			int32 Depth = 0;
			int32 Current = RefSkeleton.GetParentIndex(BoneIndex);
			while (Current != INDEX_NONE) {
				Depth++;
				Current = RefSkeleton.GetParentIndex(Current);
			}
			return Depth;
			};
		return GetDepth(A) < GetDepth(B);
		});

	// FIXED: Build reference skeleton with proper parent relationships
	for (int32 OldIndex : SortedBones)
	{
		if (!RefSkeleton.IsValidIndex(OldIndex)) continue;

		FMeshBoneInfo BoneInfo = RefSkeleton.GetRefBoneInfo()[OldIndex];
		FTransform BoneTransform = RefSkeleton.GetRefBonePose()[OldIndex];

		// Fix parent index to point to new bone index
		int32 OldParentIdx = BoneInfo.ParentIndex;
		if (OldParentIdx != INDEX_NONE && OutBoneRemap.Contains(OldParentIdx))
		{
			BoneInfo.ParentIndex = OutBoneRemap[OldParentIdx];
		}
		else
		{
			BoneInfo.ParentIndex = INDEX_NONE; // Root bone in new skeleton
		}

		int32 NewIndex = OutBoneRemap.Num();
		OutBoneRemap.Add(OldIndex, NewIndex);

		Modifier.Add(BoneInfo, BoneTransform);

		UE_LOG(LogTemp, Warning, TEXT("✅ Added bone: %s [OldIdx: %d -> NewIdx: %d, Parent: %d]"),
			*BoneInfo.Name.ToString(), OldIndex, NewIndex, BoneInfo.ParentIndex);
	}

	// Rebuild reference skeleton
	OutLimbMesh->GetRefSkeleton().RebuildRefSkeleton(SkeletalMesh->GetSkeleton(), true);

	// FIXED: Set proper root transform if needed
	if (SortedBones.Num() > 0)
	{
		int32 OldRoot = SortedBones[0];
		if (OutBoneRemap.Contains(OldRoot))
		{
			int32 NewRoot = OutBoneRemap[OldRoot];
			FTransform WorldBindPose = GetAccumulatedTransform(SkeletalMesh->GetRefSkeleton(), OldRoot);
			Modifier.UpdateRefPoseTransform(NewRoot, WorldBindPose);
		}
	}*/
#pragma endregion
}

// Used in the CreateSkeletalLimbData function for accumulating the parent bone bindpose transforms and ensuring the new root bone has the correct mesh position
FTransform ULimbExtractorTool::GetAccumulatedTransform(const FReferenceSkeleton& RefSkeleton, int32 BoneIndex)
{
	FTransform Accumulated = FTransform::Identity;
	int32 i = RefSkeleton.GetParentIndex(BoneIndex);
	
	while (RefSkeleton.IsValidIndex(BoneIndex))
	{
		/*FString s = RefSkeleton.GetBoneName(BoneIndex).ToString();
		UE_LOG(LogTemp, Warning, TEXT("Accumulating from - %s"), *s);*/
		Accumulated *= RefSkeleton.GetRefBonePose()[BoneIndex];// *= , not transform * accumulated;
		BoneIndex = RefSkeleton.GetParentIndex(BoneIndex);
		

	}
	return Accumulated;
}

// Takes all the collected info and creates the new skeletal mesh and skins it, then calls SpawnSkelMeshActor 
void ULimbExtractorTool::CreateLimbMeshFromExtractedData(USkeletalMesh* SourceMesh, USkeletalMesh*& OutLimbMesh,const TArray<FVector3f>& Vertices,const TArray<FVector2D>& UVs,	const TArray<FVector>& Normals,	const TArray<FColor>& Colors,	const TArray<FVector>& Tangents,	const TArray<FIntVector>& Triangles,	const TMap<int32, int32>& BoneRemap,	const TArray<FSkinWeightInfo>& SkinWeights,	TArray<int32>& OriginalVertexIndices)
{
#if WITH_EDITORONLY_DATA
	if (!SourceMesh || Vertices.Num() == 0 || Triangles.Num() == 0 || BoneRemap.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Invalid mesh input"));
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

	// FIXED: Build BoneMap from actual remapped bones
	TSet<uint16> BonesInUse; //was tset<uint32>
	for (const auto& Pair : BoneRemap)
	{
		BonesInUse.Add(Pair.Value); // Add new bone indices
	}
	Section.BoneMap = BonesInUse.Array();
	Section.BoneMap.Sort(); // Ensure consistent ordering

	// Create reverse lookup for BoneMap
	TMap<int32, int32> BoneToCompactIndex;
	for (int32 i = 0; i < Section.BoneMap.Num(); ++i)
	{
		BoneToCompactIndex.Add(Section.BoneMap[i], i);
	}

	// FIXED: Process vertices with proper skin weight alignment
	for (int32 i = 0; i < Vertices.Num(); ++i)
	{
		FSoftSkinVertex& V = Section.SoftVertices[i];
		V.Position = Vertices[i];
		V.TangentZ = FVector3f(Normals.IsValidIndex(i) ? Normals[i] : FVector::UpVector);
		V.TangentX = FVector3f(Tangents.IsValidIndex(i) ? Tangents[i] : FVector::ForwardVector);
		V.TangentY = FVector3f(FVector::CrossProduct(FVector(V.TangentZ), FVector(V.TangentX)));
		V.Color = Colors.IsValidIndex(i) ? Colors[i] : FColor::White;
		V.UVs[0] = FVector2f(UVs.IsValidIndex(i) ? UVs[i] : FVector2D::ZeroVector);

		// Initialize all influences
		for (int32 j = 0; j < MAX_TOTAL_INFLUENCES; ++j)
		{
			V.InfluenceBones[j] = 0;
			V.InfluenceWeights[j] = 0;
		}

		// FIXED: Get skin weights using original vertex index
		int32 OriginalIndex = OriginalVertexIndices.IsValidIndex(i) ? OriginalVertexIndices[i] : i;

		if (SkinWeights.IsValidIndex(OriginalIndex))
		{
			const FSkinWeightInfo& WeightInfo = SkinWeights[OriginalIndex];
			uint32 TotalWeight = 0;
			int32 ValidInfluences = 0;

			// Process each influence
			for (int32 j = 0; j < MAX_TOTAL_INFLUENCES && ValidInfluences < MAX_TOTAL_INFLUENCES; ++j)
			{
				uint8 OldBoneIndex = WeightInfo.InfluenceBones[j];
				uint8 Weight = WeightInfo.InfluenceWeights[j];

				if (Weight > 0)
				{
					// CRITICAL: Remap old bone index to new bone index
					if (const int32* NewBoneIndex = BoneRemap.Find(OldBoneIndex))
					{
						// Find compact index in BoneMap
						if (const int32* CompactIndex = BoneToCompactIndex.Find(*NewBoneIndex))
						{
							V.InfluenceBones[ValidInfluences] = static_cast<uint8>(*CompactIndex);
							V.InfluenceWeights[ValidInfluences] = Weight;
							TotalWeight += Weight;
							ValidInfluences++;
						}
						else
						{
							//UE_LOG(LogTemp, Warning, TEXT("⚠️ Bone %d not found in BoneMap"), *NewBoneIndex);
						}
					}
					else
					{
						//UE_LOG(LogTemp, Warning, TEXT("⚠️ No remap found for bone %d"), OldBoneIndex);
					}
				}
			}

			// FIXED: Handle cases with no valid influences or incorrect total weight
			if (ValidInfluences == 0 || TotalWeight == 0)
			{
				// Fallback to first bone in BoneMap
				V.InfluenceBones[0] = 0;
				V.InfluenceWeights[0] = 255;
				UE_LOG(LogTemp, Warning, TEXT("⚠️ Vertex %d assigned fallback weight"), i);
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

				// Final adjustment to ensure exact total of 255
				if (NewTotal != 255 && ValidInfluences > 0)
				{
					int32 Diff = 255 - NewTotal;
					V.InfluenceWeights[0] = FMath::Clamp(V.InfluenceWeights[0] + Diff, 1, 255);
				}
			}
		}
		else
		{
			// No skin weight data available - assign to root bone
			V.InfluenceBones[0] = 0;
			V.InfluenceWeights[0] = 255;
			UE_LOG(LogTemp, Warning, TEXT("⚠️ No skin weight data for vertex %d"), i);
		}
	}

	// Set up triangles
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

	// Set required bones
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

	// Add LODInfo
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

	// Copy material from source
	UMaterialInterface* Mat = SourceMesh->GetMaterials().IsValidIndex(0) ?
	SourceMesh->GetMaterials()[0].MaterialInterface : nullptr;
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
	UE_LOG(LogTemp, Error, TEXT("❌ WITH_EDITORONLY_DATA required"));
#endif

}

// This uses our colleced per-vertex data, and feeds it to the generated skeletal mesh for skinning, as well as using the buffers to create the mesh
void ULimbExtractorTool::CreateLimbMeshFromRawVertexData(USkeletalMesh* SourceMesh,USkeletalMesh*& OutLimbMesh,const TArray<FVector3f>& Vertices,const TArray<FVector2D>& UVs,const TArray<FVector>& Normals,const TArray<FColor>& Colors,const TArray<FVector>& Tangents,const TArray<FIntVector>& Triangles, TArray<FRawVertexData> VertexData,	const TMap<int32, int32>& BoneRemap, const TSet<int32>& SkinWeightBoneIndicesUsed)
{
#if WITH_EDITORONLY_DATA
	if (!SourceMesh || Vertices.Num() == 0 || Triangles.Num() == 0 || VertexData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Invalid mesh input, or empty array - V %d, T %d VD %d "), Vertices.Num(), Triangles.Num(), VertexData.Num());
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

	UE_LOG(LogTemp, Warning, TEXT("Array lengths in BuildfromVertexData are - SBIU %d, BR %d"), SkinWeightBoneIndicesUsed.Num(), BoneRemap.Num());
	const FReferenceSkeleton& NewRefSkeleton = OutLimbMesh->GetRefSkeleton();
	
	
	TSet<uint16> BonesInUse;
	for (const FRawVertexData& RawData : VertexData)
	{
		for (int32 BoneIndex : RawData.BoneInfluences)
		{
			if (BoneIndex >= 0 && NewRefSkeleton.IsValidIndex(BoneIndex))
			{
				BonesInUse.Add(BoneIndex);
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("⚠️ Invalid bone index %d in RawData, not found in RefSkeleton"), BoneIndex);
			}
		}
	}
	
	
	Section.BoneMap = BonesInUse.Array();
	Section.BoneMap.Sort(); // Ensure consistent ordering
	
	// Ensure RequiredBones includes all bones used by the mesh
	for (int32 BoneIdx : Section.BoneMap)
	{
		if (NewRefSkeleton.IsValidIndex(BoneIdx))
		{
			LODModel->RequiredBones.AddUnique(BoneIdx);
			LODModel->ActiveBoneIndices.AddUnique(BoneIdx);
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("⚠️ Bone index %d in BoneMap is not in RefSkeleton!"), BoneIdx);
		}
	}


	// Create reverse lookup for BoneMap
	TMap<int32, int32> BoneToCompactIndex;
	for (int32 i = 0; i < Section.BoneMap.Num(); ++i)
	{
		BoneToCompactIndex.Add(Section.BoneMap[i], i);
	}

	// FIXED: Process vertices with proper skin weight alignment
	for (int32 i = 0; i < Vertices.Num(); ++i)
	{
		const FRawVertexData& RawData = VertexData[i];

		FSoftSkinVertex& V = Section.SoftVertices[i];
		V.Position = Vertices[i];
		V.TangentZ = FVector3f(Normals.IsValidIndex(i) ? Normals[i] : FVector::UpVector);
		V.TangentX = FVector3f(Tangents.IsValidIndex(i) ? Tangents[i] : FVector::ForwardVector);
		V.TangentY = FVector3f(FVector::CrossProduct(FVector(V.TangentZ), FVector(V.TangentX)));
		V.Color = Colors.IsValidIndex(i) ? Colors[i] : FColor::White;
		V.UVs[0] = FVector2f(UVs.IsValidIndex(i) ? UVs[i] : FVector2D::ZeroVector);

		// Reset influences
		for (int32 j = 0; j < MAX_TOTAL_INFLUENCES; ++j)
		{
			V.InfluenceBones[j] = 0;
			V.InfluenceWeights[j] = 0;
		}

		int32 ValidInfluences = 0;
		float TotalWeight = 0.0f;

		// Use RawData directly
		for (int32 j = 0; j < RawData.BoneInfluences.Num() && ValidInfluences < MAX_TOTAL_INFLUENCES; ++j)
		{
			const int32 BoneIndex = RawData.BoneInfluences[j];
			const float Weight = RawData.BoneWeights[j];

			if (Weight <= 0.f) continue;

			const int32* CompactIndex = BoneToCompactIndex.Find(BoneIndex);
			if (!CompactIndex)
			{
				//UE_LOG(LogTemp, Warning, TEXT("⚠️ Bone index %d not in BoneMap"), BoneIndex);
				continue;
			}

			V.InfluenceBones[ValidInfluences] = static_cast<uint8>(*CompactIndex);
			V.InfluenceWeights[ValidInfluences] = FMath::Clamp(FMath::RoundToInt(Weight * 255.0f), 0, 255);
			TotalWeight += V.InfluenceWeights[ValidInfluences];
			ValidInfluences++;
		}

		// Normalize or fallback
		if (ValidInfluences == 0 || TotalWeight == 0)
		{
			V.InfluenceBones[0] = 0;
			V.InfluenceWeights[0] = 255;
			UE_LOG(LogTemp, Warning, TEXT("⚠️ Vertex %d has no valid skin weight, assigning fallback"), i);
		}
		else if (FMath::Abs(TotalWeight - 255.f) > 1.f)
		{
			float Scale = 255.f / TotalWeight;
			int32 NewTotal = 0;

			for (int32 j = 0; j < ValidInfluences; ++j)
			{
				V.InfluenceWeights[j] = FMath::Clamp(FMath::RoundToInt(V.InfluenceWeights[j] * Scale), 1, 255);
				NewTotal += V.InfluenceWeights[j];
			}

			// Fix rounding error
			if (NewTotal != 255)
			{
				int32 Diff = 255 - NewTotal;
				V.InfluenceWeights[0] = FMath::Clamp(V.InfluenceWeights[0] + Diff, 1, 255);
			}
		}
	}

	// Set up triangles
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

	// Set bounds
	FBox Box(ForceInit);
	for (const FVector3f& V : Vertices)
	{
		Box += FVector(V);
	}
	OutLimbMesh->SetImportedBounds(FBoxSphereBounds(Box));

	// Add LODInfo
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

	// Copy material from source
	UMaterialInterface* Mat = SourceMesh->GetMaterials().IsValidIndex(0) ? SourceMesh->GetMaterials()[0].MaterialInterface : nullptr;
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
	UE_LOG(LogTemp, Error, TEXT("❌ WITH_EDITORONLY_DATA required"));
#endif

}

// this makes sure our bone chain has a proper parent/child relationship ouutput to feed the mesh creation
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

// the main functions for extracting vertices, process data and building the skeletal mesh, fade rings, and gore caps - also spawns the preview actor
void ULimbExtractorTool::CreateSkeletalMesh(USkeletalMesh* SkeletalMesh,FName BoneName, bool CreateBoneChain)
{
	// add some buffers
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

#pragma region skelmesh creation
	
		TArray<int32> OutIndicies;
		TMap<int32, int32> BoneReMap;
		TArray<FRawVertexData> VertexData;

		// gets the bone indices associated with the Desired Bone Names children
		GetBoneHierachy(SkeletalMesh, BoneName, ExclBones, OutIndicies);
		if (CreateBoneChain)
		{
			// Extract the whole bone chain, or a selection of it if specified - i could make this happen without the loop, but its nice to check each bone
			for (int Index : OutIndicies)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Our chosen Bone chain starts at - %s"), *SkeletalMesh->GetRefSkeleton().GetBoneName(Index).ToString());
				FName DesiredBone = SkeletalMesh->GetRefSkeleton().GetBoneName(Index);
				ExtractSkelMeshVertices(SkeletalMesh, DesiredBone, VertexData, MinimumWeight);
			}
		}
		else
		{
			ExtractSkelMeshVertices(SkeletalMesh, BoneName, VertexData, MinimumWeight);

		}
		// Super important !! If we have no vertex data, the system will crashh and hang due to no SkeletalRenderData
		if (VertexData.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Returning due to insufficient vertex data"));
			return;
		}

		TSet<int32> SkinWeightBoneIndicesUsed;  // we may not need this anymore..
		// this creates the skeleton bone chain for the mesh using FReferenceSkeleton
		CreateSkeletalLimbData(SkeletalMesh, OutLimbSkelMesh, OutIndicies, BoneReMap, SkinWeightBoneIndicesUsed);

		//  Remap bone indices in RawVertexData to match new skeleton
		for (FRawVertexData& RawData : VertexData)
		{
			for (int32& BoneIndex : RawData.BoneInfluences)
			{
				if (const int32* NewIndex = BoneReMap.Find(BoneIndex))
				{
					BoneIndex = *NewIndex;
				}
			}
		}

		//  remove invalid weights
		for (FRawVertexData& RawData : VertexData)
		{
			for (int32 i = RawData.BoneInfluences.Num() - 1; i >= 0; --i)
			{
				if (RawData.BoneInfluences[i] == INDEX_NONE)
				{
					RawData.BoneInfluences.RemoveAt(i);
					RawData.BoneWeights.RemoveAt(i);
				}
			}
		}
		// these buffers are to make sure the correct indices line up with the skinweights
		for (const FRawVertexData Influences : VertexData)
		{
			int32 num = Influences.BoneInfluences.Num();
			for (int32 i = 0; i < num; i++)
			{
				int32 inf = Influences.BoneInfluences[i];
				SkinWeightBoneIndicesUsed.FindOrAdd(inf);

			}
			Sync.Add(Influences.RemappedIndex); // for when we mess around with the initial CreateLimbMesh function, to sync bones indices and skinweights
		}

		// builds the assorted buffers needed for the mesh creation
		BuildBuffersFromRawVertexData(SkeletalMesh, VertexData, FinalVertices, FinalNormals, FinalUVs, FinalColors, FinalTangents, FinalVertexIndices, Skin, IntVecTriangles, 0);
		//// this creates the skeleton bone chain for the mesh using FReferenceSkeleton
		CreateSkeletalLimbData(SkeletalMesh, OutLimbSkelMesh, OutIndicies, BoneReMap, SkinWeightBoneIndicesUsed);
		///  Double check this and why the array out of bounds error occurred... would be better than having all these buffres instead of utilizing the actual vertexData array
		CreateLimbMeshFromRawVertexData(SkeletalMesh, OutLimbSkelMesh, FinalVertices, FinalUVs, FinalNormals, FinalColors, FinalTangents, IntVecTriangles, VertexData, BoneReMap, SkinWeightBoneIndicesUsed);
		// obviously spawns out actor and attaches the skeletal mesh to it so we have something to inspect
		SpawnSkelMeshActor();
		//After each build, clear the array for a fresh start on the next mesh build ** pain in the ass tho to remember re-adding the child-depth bone 
		ExcludedBoneNames.Empty();

#pragma endregion

		if (AddCap) 
		{
#pragma region Construct cap data
			
			//UE_LOG(LogTemp, Warning, TEXT("Running cap mesh section"));
			UMeshCleanUtils::UtilCleanMeshForCapping(FinalVertices, IntVecTriangles, FinalUVs, FinalNormals, .01f, 10, false);

			////// Now we try to find the open edges of the mesh we extracted and sort the edges into loops
			////// Then we try to generate a cap out of triangles to attach to the mesh

			// A quick conversion to plug into the findedgeloop function
			TArray<int32> TempTriangles;
			for (const FIntVector V : IntVecTriangles)
			{
				TempTriangles.Add(V.X);
				TempTriangles.Add(V.Y);
				TempTriangles.Add(V.Z);
			}
			// Find open edges and group into loops
			TSet<TPair<int32, int32>> OpenEdges = FindOpenEdgesFromTriangles(TempTriangles);

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
			TArray<int32> FadeTriangles;
			TArray<FVector> CapNormals;
			TArray<FVector2D> CapUVs;
			TArray<FLinearColor> CapColors;
			TArray<FProcMeshTangent> CapTangents;
			TArray<TArray<FVector3f>> FoundEdgeLoops;
			TArray<TArray<FVector3f>> DomeEdgeLoops;
			TMap<int32, TArray<int32>> MaterialToTriangles;
			TArray<TArray<int32>> FadeRings;

			for (const TArray<int32>& Loop : EdgeLoops)
			{
				TArray<FVector3f> LoopPositions;
				TArray<FVector2D> LoopUVs;

				for (int32 Index : Loop)
				{
					if (FinalVertices.IsValidIndex(Index))
					{
						LoopPositions.Add(FinalVertices[Index]);
						LoopUVs.Add(FinalUVs[Index]); // aligns positions and UVs
					}
				}

				int32 LoopMaterialIndex = InferLoopMaterialFromPositions(LoopPositions, FinalTriangles, FinalVertices, MaterialIndices, 0.001f);

				TArray<int32> FadeRingLoop;
				TArray<int32> FadeRingTriangles;

				//GenerateFadeRingCapFromLoopInfo(LoopInfo, CapVertices, FadeTriangles, CapUVs, CapNormals, FadeRingLoop, FadeRingTriangles);


				MaterialToTriangles.FindOrAdd(LoopMaterialIndex).Append(FadeRingTriangles);
				FadeRings.Add(FadeRingLoop);
				// we take our output fadering and add it to the capverts needed to pass to the dome generator to build off
				// - these verts are added to finalverts inside generatefadering, but we check anyway to avoid an accidental out of bounds
				for (int32 Index : FadeRingLoop)
				{
					// check to make sure the fadering edge loop vert is in finalvertices 
					if (FinalVertices.IsValidIndex(Index))
					{
						// add it to the capverts 
						CapVertices.Add(FinalVertices[Index]);
						CapUVs.Add(FinalUVs[Index]);
						CapNormals.Add(FVector(1, 0, 0));
						CapColors.Add(FColor::Red);
						CapTangents.Add(FProcMeshTangent(1, 0, 0));

					}
				}

			}
			for (const TArray<int32> Ring : FadeRings)
			{
				TArray<FVector3f> RingVerts;
				for (int32 Index : Ring)
				{
					RingVerts.Add(CapVertices[Index]);
				}
				DomeEdgeLoops.Add(RingVerts); // -  input for dome generation
				//UE_LOG(LogTemp, Warning, TEXT("FadeEdgeLoops = %d"), DomeEdgeLoops.Num());
			}

			// finally generate the vertices data we want to give to create mesh section
			if (AddDome)
			{
				for (const TArray<FVector3f>& Loop1 : DomeEdgeLoops)
				{
					GenerateDomedCapFromLoop(Loop1, CapVertices, CapTriangles, CapUVs, CapNormals, BP_DomeHieght, BP_NoiseAmount, BP_RingCount, BP_AddNoise);
					UE_LOG(LogTemp, Warning, TEXT("generate dome called "))
				}
			}
			else
			{
				for (const TArray<FVector3f>& Loop1 : DomeEdgeLoops)
				{
					GenerateCapFanFromLoop(Loop1, CachedMeshContext.FinalVertices, CapVertices, CapTriangles, CapUVs, CapNormals);
				}

			}

			if (ShowLoopDebug)
			{
				DrawEdgeLoopsDebug(DomeEdgeLoops, FinalVertices, GetWorld(), SkelMesh->GetComponentTransform(), 20, .4);
			}

#pragma endregion

#pragma region fill CapBuffers and generate uv data

			// Prepare buffers
			TArray<FVector> CapVerts;

			for (int32 i = 0; i < CapVertices.Num(); ++i)
			{
				CapVerts.Add(FVector(CapVertices[i]));
				CapNormals.Add(FVector(1, 0, 0)); // Placeholder normal
				// generated in our dome generator
				//CapUVs.Add(FVector2D(0.5f, 0.5f));
				CapColors.Add(FLinearColor::Red);
				CapTangents.Add(FProcMeshTangent(1, 0, 0));

			}

#pragma endregion

			// WE MUST HAVE FILLED ARRAYS FROM PREVIOUS CODE FOR CAPVERTS, CAPTRIANGLES, and the others
# pragma region create capmesh for skelmesh
	// --- CREATE THE MESH ---
			UProceduralMeshComponent* CapMesh = NewObject<UProceduralMeshComponent>(OwnerActor);

			CapMesh->RegisterComponentWithWorld(GetWorld());
			CapMesh->AttachToComponent(SkelMesh, FAttachmentTransformRules::KeepRelativeTransform);
			CapMesh->SetWorldLocation(SkelMesh->GetComponentLocation());
			CapMesh->bUseComplexAsSimpleCollision = true;
			CapMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			CapMesh->SetVisibility(true);


			int32 SectionIndex = 0;

			// 1. Add all Fade Ring triangles (with inherited materials)
			for (const TPair<int32, TArray<int32>>& Pair : MaterialToTriangles)
			{
				int32 MaterialIndex = Pair.Key;
				const TArray<int32>& SectionTris = Pair.Value;

				CapMesh->CreateMeshSection_LinearColor(
					SectionIndex,
					CapVerts,
					SectionTris,
					CapNormals,
					CapUVs,
					CapColors,
					CapTangents,
					true
				);

				if (SkeletalMesh->GetMaterials().IsValidIndex(MaterialIndex))
				{
					CapMesh->SetMaterial(SectionIndex, SkeletalMesh->GetMaterials()[MaterialIndex].MaterialInterface);
				}

				++SectionIndex;
			}

			// 2. Add dome geometry (gore section)
			if (BP_CapMaterial && CapTriangles.Num() > 0)
			{
				CapMesh->CreateMeshSection_LinearColor(
					SectionIndex,
					CapVerts,
					CapTriangles,
					CapNormals,
					CapUVs,
					CapColors,
					CapTangents,
					true
				);

				CapMesh->SetMaterial(SectionIndex, BP_CapMaterial.Get());
			}

#pragma endregion
			// all done
		}
		return;

	
#pragma endregion
}

void ULimbExtractorTool::CreateStaticMesh(USkeletalMesh* SkeletalMesh, FName BoneName, bool CreateBoneChain)
{
		// add some buffers
	TSet<int32> SelectedVertices;

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
	TMap<int32, TArray<int32>> OutMaterialToTriangles;
	#pragma region Static Mesh creation
	
	// If we are only extracting a static mesh, then this section does the extraction and capping --

	TArray<int32> OutIndicies;
	TMap<int32, int32> BoneReMap;

	// set our mesh for later
	InMesh = SkeletalMesh;

	// gets the bone indices associated with the Desired Bone Names children
	GetBoneHierachy(SkeletalMesh, BoneName, ExclBones, OutIndicies);

	TArray<int32> BoneIndices;
	GetBoneHierachy(SkeletalMesh, BoneName, ExclBones, BoneIndices);
	if (CreateBoneChain) // get the whole bone chain worth of verts
	{
		// Extract the whole bone chain, or a selection of it if specified - i could make this happen without the loop, but its nice to check each bone
		for (int Index : OutIndicies)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Our chosen Bone chain starts at - %s"), *SkeletalMesh->GetRefSkeleton().GetBoneName(Index).ToString());
			FName DesiredBone = SkeletalMesh->GetRefSkeleton().GetBoneName(Index);
			// Extract the bone vertices from the mesh associated with the bone its skinned to
			ExtractBoneVertices(SkeletalMesh, DesiredBone, FinalVertices, FinalVertexIndices, MinimumWeight /*= 128*/, SelectedVertices);

		}

	}
	else  //  get only the chosen bones verts
	{
		ExtractBoneVertices(SkeletalMesh, BoneName, FinalVertices, FinalVertexIndices, MinimumWeight /*= 128*/, SelectedVertices);
	}

	// Super important !! If we have no vertex data, the system will crashh and hang due to no SkeletalRenderData
	if (FinalVertices.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Returning due to insufficient vertex data"));
		return;
	}

	//Take our vertices and build the data needed to fill the buffers for the new mesh
	BuildStaticMeshData(SkeletalMesh, FinalVertexIndices, FinalVertices, FinalUVs, FinalNormals, FinalColors, FinalTangents, OldToNewVertexMap, IntVecTriangles, MaterialIndices);
	
	// convert the tangents to FProcMeshTangents for the create mesh section - i should probably convert previous function to do this...
	TArray<FProcMeshTangent> MeshTangents;
	for (const FVector& Vector : FinalTangents)
	{
		MeshTangents.Add(FProcMeshTangent(Vector.X, Vector.Y, Vector.Z));
	}
	// Convert TempTriangles back to TArray<int32> for the mesh creation function
	FinalTriangles.Empty();
	for (const FIntVector& Vec : IntVecTriangles)
	{
		FinalTriangles.Add(Vec.X);
		FinalTriangles.Add(Vec.Y);
		FinalTriangles.Add(Vec.Z);
	}

#pragma region Material Mapping

// before creating the mesh, add a material mapping to each triangle
	TMap<int32, TArray<int32>> MaterialToTriangles;

	GenerateMaterialToTrianglesMap(MaterialIndices, FinalTriangles, MaterialToTriangles);

	// a check to make sure materials are being assigned to triangles
	TMap<int32, int32> TriangleCountPerMaterial;
	for (int32 i = 0; i < MaterialIndices.Num(); ++i)
	{
		int32 MatIdx = MaterialIndices[i];
		TriangleCountPerMaterial.FindOrAdd(MatIdx)++;
	}

	for (const TPair<int32, int32>& Pair : TriangleCountPerMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Material %d: %d triangles"), Pair.Key, Pair.Value);
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

	int32 SectionIndex = 0;;
	
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
	// fill our Cached Mesh Context so we can use it in the capping section
	CachedMeshContext.FinalMaterialIndices = MaterialIndices;
	CachedMeshContext.FinalNormals = FinalNormals;
	CachedMeshContext.FinalTangents = FinalTangents;
	CachedMeshContext.FinalTriangles = FinalTriangles;
	CachedMeshContext.FinalUVs = FinalUVs;
	CachedMeshContext.FinalVertices = FinalVertices;
	CachedMeshContext.IntVecTriangles = IntVecTriangles;
#pragma endregion
	BuildEdgeLoopInfoCache(SkeletalMesh);
	/*FTimerHandle CapDecisionTimer;

	FTimerDelegate CapCallback;
	CapCallback.BindUObject(this, &ULimbExtractorTool::BuildEdgeLoopInfoCache, SkeletalMesh);

	GetWorld()->GetTimerManager().SetTimer(CapDecisionTimer, CapCallback, 0.2f, false);*/
#pragma endregion
}

// original capping code layout in case we need to fallback
//void ULimbExtractorTool::AddFadeRingsAndCaps(USkeletalMesh* Mesh)//(TArray<FVector3f>& FinalVertices, TArray<FIntVector>& IntVecTriangles, TArray<FVector2d>& FinalUVs, TArray<FVector>& FinalNormals, TArray<int32>& FinalTriangles, TArray<int32>& MaterialIndices, TMap<int32, TArray<int32>>& MaterialToTriangles, int32& SectionIndex, USkeletalMesh* SkeletalMesh)
//{
//
//	const FString CapText = FString::Printf(TEXT("Would you like to add fade rings and gore caps to any open mesh seections?"));
//	EAppReturnType::Type CapResponse = FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString(CapText));
//	AddCap = (CapResponse == EAppReturnType::Yes);
//
//	if (AddCap)
//	{
//#pragma region Construct cap data
//
//		TArray<FVector3f>& FinalVertices = CachedMeshContext.FinalVertices;
//		TArray<FIntVector>& IntVecTriangles = CachedMeshContext.IntVecTriangles;
//		TArray<FVector2d>& FinalUVs = CachedMeshContext.FinalUVs;
//		TArray<FVector>& FinalNormals = CachedMeshContext.FinalNormals;
//		TArray<int32>& FinalTriangles = CachedMeshContext.FinalTriangles;
//		TArray<int32>& MaterialIndices = CachedMeshContext.FinalMaterialIndices;
//		TMap<int32, TArray<int32>> MaterialToTriangles;
//		int32 SectionIndex = 0;
//		USkeletalMesh* SkeletalMesh = Mesh;
//
//		//UE_LOG(LogTemp, Warning, TEXT("Running cap mesh section"));                                    // was .01
//		UMeshCleanUtils::UtilCleanMeshForCapping(FinalVertices, IntVecTriangles, FinalUVs, FinalNormals, 1.01f, 10, false);
//
//		// create a new map using the cleaned triangles
//		//GenerateMaterialToTrianglesMap(MaterialIndices, FinalTriangles, MaterialToTriangles);
//
//		////// Now we try to find the open edges of the mesh we extracted and sort the edges into loops
//		////// Then we try to generate a cap out of triangles to attach to the mesh
//
//		// A quick conversion to plug into the findedgeloop function
//		TArray<int32> TempTriangles;
//		for (const FIntVector V : IntVecTriangles)
//		{
//			TempTriangles.Add(V.X);
//			TempTriangles.Add(V.Y);
//			TempTriangles.Add(V.Z);
//		}
//		// Find open edges and group into loops
//		TSet<TPair<int32, int32>> OpenEdges = FindOpenEdgesFromTriangles(TempTriangles);
//
//		TArray<TArray<int32>> EdgeLoops = SortOpenEdgesIntoLoops(OpenEdges, true);
//
//		// this is just to make sure we have loops with enough vertices
//		TArray<TArray<int32>> CleanedLoops;
//		// this is to store our edgeloop info, incase we want to skip using them, in case of multi-meshes with overlapping geometry
//		TArray<FEdgeLoopInfo> EdgeLoopInfos;
//
//		for (const TArray<int32>& Loop : EdgeLoops)
//		{
//			if (Loop.Num() != 0)  // skip only broken loops
//			{
//				CleanedLoops.Add(Loop);
//
//				FEdgeLoopInfo Info;
//				Info.VertexIndices = Loop;
//
//				for (int32 Index : Loop)
//				{
//					if (FinalVertices.IsValidIndex(Index))
//						Info.Positions.Add(FinalVertices[Index]); // this tracks which vertices are used in the edgeloop
//				}
//
//				Info.VotedMaterialIndex = InferLoopMaterialFromPositions(Info.Positions, FinalTriangles, FinalVertices, MaterialIndices, 0.001f); // this tracks which material index the edgeloop uses
//				//Info.TotalEdgeLength = CalculateEdgeLoopLength(Info.Positions); // optional as we dont have this mapped out yet
//				Info.bShouldCap = true; // or flag selectively later
//
//				EdgeLoopInfos.Add(Info);
//
//			}
//			UE_LOG(LogTemp, Warning, TEXT("Raw Detected Loop with %d vertices"), Loop.Num());
//		}
//		for (FEdgeLoopInfo& Info : EdgeLoopInfos)
//		{
//			//DrawEdgeLoopDebug(Info.Positions, FColor::Yellow); // Neutral preview before decision
//			TArray<TArray<FVector3f>> vec;
//			vec.Add(Info.Positions);
//			DrawEdgeLoopsDebug(vec, FinalVertices, GetWorld(), PreviewMesh->GetComponentTransform(), 20, .4);
//
//			const FString PromptText = FString::Printf(
//				TEXT("Loop size: %d\nLength: %.2f\nMaterial: %d\nCap this loop?"),
//				Info.VertexIndices.Num(),
//				Info.TotalEdgeLength,
//				Info.VotedMaterialIndex
//			);
//
//			EAppReturnType::Type UserResponse = FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString(PromptText));
//			Info.bShouldCap = (UserResponse == EAppReturnType::Yes);
//
//			// Optional: Update debug color after decision
//			DrawEdgeLoopsDebug(vec, FinalVertices, GetWorld(), PreviewMesh->GetComponentTransform(), 20, .4);
//		}
//
//		EdgeLoops = CleanedLoops;
//
//		////////////////////////////////////////////////////////////////////////////////////////////
//		// Create cap vertex data and merge it with 
//		TArray<FVector3f> CapVertices;
//		TArray<int32> CapTriangles;
//		TArray<int32> FadeTriangles;
//		TArray<FVector> CapNormals;
//		TArray<FVector2D> CapUVs;
//		TArray<FLinearColor> CapColors;
//		TArray<FProcMeshTangent> CapTangents;
//		TArray<TArray<FVector3f>> FoundEdgeLoops;
//		TArray<TArray<FVector3f>> DomeEdgeLoops;
//		//TMap<int32, TArray<int32>> MaterialToTriangles;
//		TArray<TArray<int32>> FadeRings;
//
//		for (const TArray<int32>& Loop : EdgeLoops)
//		{
//			TArray<FVector3f> LoopPositions;
//			TArray<FVector2D> LoopUVs;
//
//			for (int32 Index : Loop)
//			{
//				if (FinalVertices.IsValidIndex(Index))
//				{
//					LoopPositions.Add(FinalVertices[Index]);
//					LoopUVs.Add(FinalUVs[Index]); // aligns positions and UVs
//				}
//			}
//
//			int32 LoopMaterialIndex = InferLoopMaterialFromPositions(LoopPositions, FinalTriangles, FinalVertices, MaterialIndices, 0.001f);
//
//			TArray<int32> FadeRingLoop;
//			TArray<int32> FadeRingTriangles;
//
//			GenerateFadeRingCapFromLoop(
//				LoopPositions,
//				CapVertices,
//				FadeTriangles,
//				CapUVs,
//				LoopUVs,
//				CapNormals,
//				BP_FadeRingLength,
//				FadeRingLoop,
//				FadeRingTriangles,
//				BP_AddNoise,
//				BP_NoiseAmount);
//
//			MaterialToTriangles.FindOrAdd(LoopMaterialIndex).Append(FadeRingTriangles);
//
//			FadeRings.Add(FadeRingLoop);
//			// we take our output fadering and add it to the capverts needed to pass to the dome generator to build off
//			// - these verts are added to finalverts inside generatefadering, but we check anyway to avoid an accidental out of bounds
//			for (int32 Index : FadeRingLoop)
//			{
//				// check to make sure the fadering edge loop vert is in finalvertices 
//				if (FinalVertices.IsValidIndex(Index))
//				{
//					// add it to the capverts 
//					CapVertices.Add(FinalVertices[Index]);
//					CapUVs.Add(FinalUVs[Index]);
//					CapNormals.Add(FVector(1, 0, 0));
//					CapColors.Add(FColor::Red);
//					CapTangents.Add(FProcMeshTangent(1, 0, 0));
//
//				}
//			}
//
//		}
//		for (const TArray<int32> Ring : FadeRings)
//		{
//			TArray<FVector3f> RingVerts;
//			for (int32 Index : Ring)
//			{
//				RingVerts.Add(CapVertices[Index]);
//			}
//			DomeEdgeLoops.Add(RingVerts); // -  input for dome generation
//			//UE_LOG(LogTemp, Warning, TEXT("FadeEdgeLoops = %d"), DomeEdgeLoops.Num());
//		}
//
//		// finally generate the vertices data we want to give to create mesh section
//		if (AddDome)
//		{
//			for (const TArray<FVector3f>& Loop1 : DomeEdgeLoops)
//			{
//				//GenerateDomedCapFromLoop(Loop1, CapVertices, CapTriangles, CapUVs, CapNormals, BP_DomeHieght, BP_NoiseAmount, BP_RingCount, BP_AddNoise);
//				UE_LOG(LogTemp, Warning, TEXT("generate dome called "))
//			}
//		}
//		else
//		{
//			for (const TArray<FVector3f>& Loop1 : DomeEdgeLoops)
//			{
//				//GenerateCapFanFromLoop(Loop1, FinalVertices, CapVertices, CapTriangles);
//			}
//
//		}
//
//		if (ShowLoopDebug)
//		{
//			DrawEdgeLoopsDebug(DomeEdgeLoops, FinalVertices, GetWorld(), PreviewMesh->GetComponentTransform(), 20, .4);
//		}
//
//#pragma endregion
//
//#pragma region fill CapBuffers and generate uv data
//
//		// Prepare buffers
//		TArray<FVector> CapVerts;
//
//		for (int32 i = 0; i < CapVertices.Num(); ++i)
//		{
//			CapVerts.Add(FVector(CapVertices[i]));
//			CapNormals.Add(FVector(1, 0, 0)); // Placeholder normal
//			// generated in our dome generator
//			//CapUVs.Add(FVector2D(0.5f, 0.5f));
//			CapColors.Add(FLinearColor::Red);
//			CapTangents.Add(FProcMeshTangent(1, 0, 0));
//
//		}
//
//#pragma endregion
//
//		// WE MUST HAVE FILLED ARRAYS FROM PREVIOUS CODE FOR CAPVERTS, CAPTRIANGLES, and the others
//# pragma region create capmesh for PreviewMesh
//		// --- CREATE THE MESH ---
//		UProceduralMeshComponent* CapMesh = NewObject<UProceduralMeshComponent>(OwnerActor);
//
//		CapMesh->RegisterComponentWithWorld(GetWorld());
//		CapMesh->AttachToComponent(PreviewMesh, FAttachmentTransformRules::KeepRelativeTransform);
//		CapMesh->SetWorldLocation(PreviewMesh->GetComponentLocation());
//		CapMesh->bUseComplexAsSimpleCollision = true;
//		CapMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//		CapMesh->SetVisibility(true);
//
//
//		//int32 SectionIndex = 0;
//
//		// 1. Add all Fade Ring triangles (with inherited materials)
//		for (const TPair<int32, TArray<int32>>& Pair : MaterialToTriangles)
//		{
//			int32 MaterialIndex = Pair.Key;
//			const TArray<int32>& SectionTris = Pair.Value;
//
//			CapMesh->CreateMeshSection_LinearColor(
//				SectionIndex,
//				CapVerts,
//				SectionTris,
//				CapNormals,
//				CapUVs,
//				CapColors,
//				CapTangents,
//				true
//			);
//
//			if (SkeletalMesh->GetMaterials().IsValidIndex(MaterialIndex))
//			{
//				CapMesh->SetMaterial(SectionIndex, SkeletalMesh->GetMaterials()[MaterialIndex].MaterialInterface);
//			}
//
//			++SectionIndex;
//		}
//
//		// 2. Add dome geometry (gore section)
//		if (BP_CapMaterial && CapTriangles.Num() > 0)
//		{
//			CapMesh->CreateMeshSection_LinearColor(
//				SectionIndex,
//				CapVerts,
//				CapTriangles,
//				CapNormals,
//				CapUVs,
//				CapColors,
//				CapTangents,
//				true
//			);
//
//			CapMesh->SetMaterial(SectionIndex, BP_CapMaterial.Get());
//		}
//
//#pragma endregion
//		// all done
//	}
//	return;
//}



void ULimbExtractorTool::BuildEdgeLoopInfoCache(USkeletalMesh* Mesh)//(TArray<FVector3f>& FinalVertices, TArray<FIntVector>& IntVecTriangles, TArray<FVector2d>& FinalUVs, TArray<FVector>& FinalNormals, TArray<int32>& FinalTriangles, TArray<int32>& MaterialIndices, TMap<int32, TArray<int32>>& MaterialToTriangles, int32& SectionIndex, USkeletalMesh* SkeletalMesh)
{

#pragma region Construct cap data

	TArray<FVector3f>& FinalVertices = CachedMeshContext.FinalVertices;
	TArray<FIntVector>& IntVecTriangles = CachedMeshContext.IntVecTriangles;
	TArray<FVector2d>& FinalUVs = CachedMeshContext.FinalUVs;
	TArray<FVector>& FinalNormals = CachedMeshContext.FinalNormals;
	TArray<int32>& FinalTriangles = CachedMeshContext.FinalTriangles;
	TArray<int32>& MaterialIndices = CachedMeshContext.FinalMaterialIndices;
	int32 SectionIndex = 0;
	USkeletalMesh* SkeletalMesh = Mesh;

	UMeshCleanUtils::UtilCleanMeshForCapping(FinalVertices, IntVecTriangles, FinalUVs, FinalNormals, 1.01f, 10, false);

	// A quick conversion to plug into the findedgeloop function
	TArray<int32> TempTriangles;
	for (const FIntVector V : IntVecTriangles)
	{
		TempTriangles.Add(V.X);
		TempTriangles.Add(V.Y);
		TempTriangles.Add(V.Z);
	}
	// Find open edges and group into loops
	TSet<TPair<int32, int32>> OpenEdges = FindOpenEdgesFromTriangles(TempTriangles);

	TArray<TArray<int32>> EdgeLoops = SortOpenEdgesIntoLoops(OpenEdges, true);

	// this is just to make sure we have loops with enough vertices
	TArray<TArray<int32>> CleanedLoops;
	// this is to store our edgeloop info, incase we want to skip using them, in case of multi-meshes with overlapping geometry
	TArray<FEdgeLoopInfo> EdgeLoopInfoArray;
	// something we can increment to set the Info.LoopNumber for each loop
	int32 LoopNum = 0;
	for (const TArray<int32>& Loop : EdgeLoops)
	{
			if (Loop.Num() != 0)  // skip only broken loops
		{

			CleanedLoops.Add(Loop);

			FEdgeLoopInfo Info;
			Info.VertexIndices = Loop;

			for (int32 Index : Loop)
			{
				if (FinalVertices.IsValidIndex(Index))
					Info.Positions.Add(FinalVertices[Index]); // this tracks which vertices are used in the edgeloop
					Info.UVs.Add(FinalUVs[Index]);
			}

			Info.VotedMaterialIndex = InferLoopMaterialFromPositions(Info.Positions, FinalTriangles, FinalVertices, MaterialIndices, 0.001f); // this tracks which material index the edgeloop uses
			//Info.TotalEdgeLength = CalculateEdgeLoopLength(Info.Positions); // optional as we dont have this mapped out yet
			
			Info.LoopNumber = LoopNum;
			UE_LOG(LogTemp, Warning, TEXT("Info.LoopNumber =  %d "), Info.LoopNumber);

			EdgeLoopInfoArray.Add(Info);
			LoopNum++;
		}
		UE_LOG(LogTemp, Warning, TEXT("Raw Detected Loop with %d vertices"), Loop.Num());
	}

	EdgeLoopInfos = EdgeLoopInfoArray;
	EdgeLoops = CleanedLoops;

	Wrapper.EdgeLoops = EdgeLoopInfos;
	Prompt.Broadcast(Wrapper);
	//PromptNextLoopForCapping(Mesh);
}

// called in reateFadeRingAndCap to fill the buffers for the cap mesh
void ULimbExtractorTool::FillCapBuffers(const TArray<FVector3f>& SourceVerts,const TArray<FVector2D>& SourceUVs,TArray<FVector>& OutVerts,TArray<FVector>& OutNormals,TArray<FVector2D>& OutUVs,TArray<FLinearColor>& OutColors,TArray<FProcMeshTangent>& OutTangents,const FLinearColor& Color)
{
	int32 Count = SourceVerts.Num();
	OutVerts.Reserve(Count);
	OutNormals.Reserve(Count);
	OutUVs.Reserve(Count);
	OutColors.Reserve(Count);
	OutTangents.Reserve(Count);

	for (int32 i = 0; i < Count; ++i)
	{
		OutVerts.Add(FVector(SourceVerts[i]));
		OutNormals.Add(FVector(1, 0, 0));
		//OutUVs.Add(SourceUVs.IsValidIndex(i) ? SourceUVs[i] : FVector2D(0.5f, 0.5f));
		if (!SourceUVs.IsValidIndex(i))
		{
			UE_LOG(LogTemp, Warning, TEXT("UV index %d is invalid — fallback UV used"), i);
		}

		OutColors.Add(Color);
		OutTangents.Add(FProcMeshTangent(1, 0, 0));
	}
}
// our main cappinng function that creates the fade ring and cap mesh
void ULimbExtractorTool::CreateFadeRingAndCap(FEdgeLoopInfo LoopInfo, FEdgeLoopInfo& UpdatedInfo)
{
	BP_UVScale = LoopInfo.UV_Scale;
	BP_UVRotationDegrees = LoopInfo.UV_Rotation;
	BP_UVOffset = FVector2D(LoopInfo.UV_Offset_X, LoopInfo.UV_Offset_Y);

	UE_LOG(LogTemp, Warning, TEXT("Create Cap Running, loopInfo #%d"), LoopInfo.LoopNumber);
	// Destroy old mesh if it exists
	if (CapComponents.Contains(LoopInfo.LoopNumber))
	{
		UProceduralMeshComponent* OldMesh = CapComponents[LoopInfo.LoopNumber];
		if (OldMesh && OldMesh->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroying %s %d"), *OldMesh->GetFName().ToString(), LoopInfo.LoopNumber);
			OldMesh->DestroyComponent();
		}
		CapComponents.Remove(LoopInfo.LoopNumber);
	}
	////////////////////////////////////////////////////////////////////////////////////////////
		// Create cap vertex data and merge it with 
	TArray<FVector3f> CapVertices;
	TArray<int32> CapTriangles;
	TArray<int32> FadeTriangles;
	TArray<FVector> CapNormals;
	TArray<FVector2D> CapUVs;
	TArray<FLinearColor> CapColors;
	TArray<FProcMeshTangent> CapTangents;
	TArray<TArray<FVector3f>> FoundEdgeLoops;
	TArray<TArray<FVector3f>> DomeEdgeLoops;
	TMap<int32, TArray<int32>> MaterialToTriangles;
	TArray<TArray<int32>> FadeRings;

	// extract loop positions
		TArray<FVector3f> LoopPositions;
		TArray<FVector2D> LoopUVs;

		// this gives us the uv alignment of the base edgeloop ring
		for (int32 Index : LoopInfo.VertexIndices)
		{
			if (CachedMeshContext.FinalVertices.IsValidIndex(Index))
			{
				LoopPositions.Add(CachedMeshContext.FinalVertices[Index]);
				LoopUVs.Add(CachedMeshContext.FinalUVs[Index]); // aligns positions and UVs
				
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("We have issues already assigning cachedmeshcontext!"));
			}
		}
		
		// get the material from the associated edgeloop tris
		int32 LoopMaterialIndex = InferLoopMaterialFromPositions(LoopPositions, CachedMeshContext.FinalTriangles, CachedMeshContext.FinalVertices, CachedMeshContext.FinalMaterialIndices, 0.001f);

		// generate a fade ring, or use the iput edgeloop
		if (LoopInfo.bAddFadeRing)
		{
			TArray<int32> FadeRingLoop;
			TArray<int32> FadeRingTriangles;
		
			//GenerateFadeRingCapFromLoop(LoopPositions, CapVertices, FadeTriangles, CapUVs, LoopUVs, CapNormals, LoopInfo.FadeRingHieght, FadeRingLoop, FadeRingTriangles, LoopInfo.bAddNoise, LoopInfo.NoiseAmount);
			
			GenerateFadeRingCapFromLoopInfo(LoopInfo, CapVertices, FadeTriangles, CapUVs, CapNormals, FadeRingLoop,	FadeRingTriangles);
			

			MaterialToTriangles.FindOrAdd(LoopMaterialIndex).Append(FadeRingTriangles);

			FadeRings.Add(FadeRingLoop);
			// we take our output fadering and add it to the capverts needed to pass to the dome generator to build off
			// - these verts are added to finalverts inside generatefadering, but we check anyway to avoid an accidental out of bounds
			if (FadeRingLoop.Num() > 0)
			{
				// Build dome edge loop from fade ring
				TArray<FVector3f> RingVerts;
				for (int32 Index : FadeRingLoop)
				{
					if (CapVertices.IsValidIndex(Index))
					{
						RingVerts.Add(CapVertices[Index]);
					}
				}
				DomeEdgeLoops.Add(RingVerts);
			}									
			
		}
		else
		{
			// Since we dont have a fadeloop, we just populate the DomeEdgeLoop with the original found edgeloop
			DomeEdgeLoops.Add(LoopPositions);
			CapVertices = LoopPositions;
			CapUVs = LoopUVs;
				
		}
		

		
	// final sanity check so an empty array stack doesnt get passed and crash it
	if(DomeEdgeLoops.Num() == 0 || DomeEdgeLoops[0].Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No edge loops found to cap!"));
		return;
	}

	// finally generate the vertices data we want to give to create mesh section
	if (LoopInfo.bAddDome)
	{
		for (const TArray<FVector3f>& Loop1 : DomeEdgeLoops)
		{
			GenerateDomedCapFromLoop(Loop1, CapVertices, CapTriangles, CapUVs, CapNormals, LoopInfo.DomeHeight, LoopInfo.NoiseAmount, LoopInfo.RingCount, LoopInfo.bAddNoise);
		}
	}
	else
	{
		for (const TArray<FVector3f>& Loop1 : DomeEdgeLoops)
		{
			GenerateCapFanFromLoop(Loop1, CachedMeshContext.FinalVertices, CapVertices, CapTriangles, CapUVs, CapNormals);
		}

	}

	if (ShowLoopDebug)
	{
		DrawEdgeLoopsDebug(DomeEdgeLoops, CachedMeshContext.FinalVertices, GetWorld(), PreviewMesh->GetComponentTransform(), 20, .4);
	}

#pragma endregion

#pragma region fill CapBuffers and generate uv data

	UE_LOG(LogTemp, Warning, TEXT("SourceVerts: %d, SourceUVs: %d"), CapVertices.Num(), CapUVs.Num());
	// --- Fill Buffers ---
	TArray<FVector> CapVerts;
	//FillCapBuffers(CapVertices,CachedMeshContext.FinalUVs,CapVerts,CapNormals,CapUVs,CapColors,CapTangents,FLinearColor::Red);
	for (int32 i = 0; i < CapVertices.Num(); ++i)
				{
					CapVerts.Add(FVector(CapVertices[i]));
					CapNormals.Add(FVector(1, 0, 0)); // Placeholder normal
					// generated in our dome generator
					//CapUVs.Add(FVector2D(0.5f, 0.5f));
					CapColors.Add(FLinearColor::Red);
					CapTangents.Add(FProcMeshTangent(1, 0, 0));
		
				}
#pragma endregion

	// WE MUST HAVE FILLED ARRAYS FROM PREVIOUS CODE FOR CAPVERTS, CAPTRIANGLES, and the others
# pragma region create capmesh
		// --- CREATE THE MESH ---
	FString Name = FString::Printf(TEXT("CapMesh_%d"), LoopInfo.LoopNumber);
	UProceduralMeshComponent* CapMesh = NewObject<UProceduralMeshComponent>(OwnerActor,*Name);

	CapMesh->RegisterComponentWithWorld(GetWorld());
	CapMesh->AttachToComponent(PreviewMesh, FAttachmentTransformRules::KeepRelativeTransform);
	CapMesh->SetWorldLocation(PreviewMesh->GetComponentLocation());
	CapMesh->bUseComplexAsSimpleCollision = true;
	CapMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapMesh->SetVisibility(true);

	int32 SectionIndex = 0;

	// 1. Add all Fade Ring triangles (with inherited materials)
	for (const TPair<int32, TArray<int32>>& Pair : MaterialToTriangles)
	{
		int32 MaterialIndex = Pair.Key;
		const TArray<int32>& SectionTris = Pair.Value;

		CapMesh->CreateMeshSection_LinearColor(
			SectionIndex,
			CapVerts,
			SectionTris,
			CapNormals,
			CapUVs,
			CapColors,
			CapTangents,
			true
		);

		if /*(BP_CapMaterial)*/(InMesh->GetMaterials().IsValidIndex(MaterialIndex))
		{
			CapMesh->SetMaterial(SectionIndex, /*BP_CapMaterial.Get());*/ InMesh->GetMaterials()[MaterialIndex  ].MaterialInterface);
		}
		UE_LOG(LogTemp, Warning, TEXT("Section %d: MaterialIndex = %d, TriCount = %d"),
			SectionIndex,
			MaterialIndex,
			SectionTris.Num());

		if (InMesh->GetMaterials().IsValidIndex(MaterialIndex))
		{
			UMaterialInterface* Mat = InMesh->GetMaterials()[MaterialIndex].MaterialInterface;
			if (Mat)
			{
				UE_LOG(LogTemp, Warning, TEXT("Assigned Material: %s"), *Mat->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("MaterialInterface is null for index %d"), MaterialIndex);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid MaterialIndex %d for base mesh"), MaterialIndex);
		}

		++SectionIndex;

	}

	// 2. Add capping geometry (gore/cap section)
	if (CapTriangles.Num() > 0)
	{
		CapMesh->CreateMeshSection_LinearColor(
			SectionIndex,
			CapVerts,
			CapTriangles,
			CapNormals,
			CapUVs,
			CapColors,
			CapTangents,
			true
		);
		if (BP_CapMaterial)
		{
			CapMesh->SetMaterial(SectionIndex, BP_CapMaterial.Get());
		}
		else
		{
			UMaterial* DefaultMaterial = nullptr;

			if (GEngine)
			{
				DefaultMaterial = GEngine->DebugEditorMaterial;
			}

			if (DefaultMaterial)
			{
				UE_LOG(LogTemp, Log, TEXT("Successfully retrieved the default world material: %s"), *DefaultMaterial->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve the default world material."));
			}
			
			CapMesh->SetMaterial(SectionIndex, DefaultMaterial);
		}
	}
	
	// Store new mesh in map
	CapComponents.Add(LoopInfo.LoopNumber, CapMesh);


	UpdatedInfo = LoopInfo;
	Wrapper.EdgeLoops.Empty();
#pragma endregion
	// all done
}

// The new  buffer filler for the skelmesh 
void ULimbExtractorTool::BuildBuffersFromRawVertexData(USkeletalMesh* SourceMesh,const TArray<FRawVertexData>& VertexData,TArray<FVector3f>& OutVertices,TArray<FVector>& OutNormals,TArray<FVector2D>& OutUVs,TArray<FColor>& OutColors,TArray<FVector>& OutTangents,TArray<int32>& OutOriginalVertexIndices,TArray<FSkinWeightInfo>& OutSkinWeights,TArray<FIntVector>& OutTriangles,int32 LODIndex)
{
	if (!SourceMesh || VertexData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Invalid skeletal mesh or empty vertex data"));
		return;
	}
	const FSkeletalMeshRenderData* RenderData = SourceMesh->GetResourceForRendering();
	if (!RenderData || RenderData->LODRenderData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Invalid render data on source mesh."));
		return;
	}
	const FSkeletalMeshLODRenderData& LODData = SourceMesh->GetResourceForRendering()->LODRenderData[LODIndex];
	const FStaticMeshVertexBuffers& Buffers = LODData.StaticVertexBuffers;
	const FSkinWeightVertexBuffer* SkinWeightBuffer = &LODData.SkinWeightVertexBuffer;
	const FRawStaticIndexBuffer16or32Interface* IndexBuffer = LODData.MultiSizeIndexContainer.GetIndexBuffer();
	if (!IndexBuffer)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Index buffer is null"));
		return;
	}
	// Build lookup: OriginalVertexIndex → NewIndex in OutVertices
	TMap<int32, int32> OldToNewIndex;
	for (int32 i = 0; i < VertexData.Num(); ++i)
	{
		const FRawVertexData& V = VertexData[i];
		OutVertices.Add(FVector3f(V.Position));
		OutOriginalVertexIndices.Add(V.OriginalVertexIndex);
		OldToNewIndex.Add(V.OriginalVertexIndex, i);

		/*OutNormals.Add(FVector::UpVector);
		OutTangents.Add(FVector::ForwardVector);
		OutUVs.Add(FVector2D::ZeroVector);
		OutColors.Add(FColor::White);*/
		//OutVertices.Add(Buffers.PositionVertexBuffer.VertexPosition(OldIndex));
		OutNormals.Add(FVector(Buffers.StaticMeshVertexBuffer.VertexTangentZ(V.OriginalVertexIndex)));
		OutTangents.Add(FVector(Buffers.StaticMeshVertexBuffer.VertexTangentX(V.OriginalVertexIndex)));
		OutColors.Add(FColor::White);
		OutUVs.Add(FVector2D(Buffers.StaticMeshVertexBuffer.GetVertexUV(V.OriginalVertexIndex, 0)));

		// Build skin weight
		FSkinWeightInfo WeightInfo;
		FMemory::Memzero(WeightInfo);

		int32 InfluenceCount = FMath::Min(V.BoneInfluences.Num(), MAX_TOTAL_INFLUENCES);
		uint32 TotalWeight = 0;

		for (int32 j = 0; j < InfluenceCount; ++j)
		{
			WeightInfo.InfluenceBones[j] = static_cast<uint8>(V.BoneInfluences[j]);
			WeightInfo.InfluenceWeights[j] = static_cast<uint8>(FMath::Clamp(V.BoneWeights[j] * 255.0f, 0.0f, 255.0f));
			TotalWeight += WeightInfo.InfluenceWeights[j];
		}

		// Normalize if needed
		if (TotalWeight != 255 && TotalWeight > 0)
		{
			float Scale = 255.0f / TotalWeight;
			uint32 NewTotal = 0;
			for (int32 j = 0; j < InfluenceCount; ++j)
			{
				WeightInfo.InfluenceWeights[j] = FMath::Clamp(FMath::RoundToInt(WeightInfo.InfluenceWeights[j] * Scale), 1, 255);
				NewTotal += WeightInfo.InfluenceWeights[j];
			}
			int32 Diff = 255 - NewTotal;
			WeightInfo.InfluenceWeights[0] = FMath::Clamp(WeightInfo.InfluenceWeights[0] + Diff, 1, 255);
		}

		OutSkinWeights.Add(WeightInfo);
	}

	// Build triangle indices from original mesh
	const int32 NumIndices = IndexBuffer->Num();
	for (int32 i = 0; i < NumIndices; i += 3)
	{
		int32 I0 = IndexBuffer->Get(i);
		int32 I1 = IndexBuffer->Get(i + 1);
		int32 I2 = IndexBuffer->Get(i + 2);

		const int32* NewI0 = OldToNewIndex.Find(I0);
		const int32* NewI1 = OldToNewIndex.Find(I1);
		const int32* NewI2 = OldToNewIndex.Find(I2);

		if (NewI0 && NewI1 && NewI2)
		{
			OutTriangles.Add(FIntVector(*NewI0, *NewI1, *NewI2));
		}
	}
}

// get our collected mesh data and convert it to useful data to plug into the createmeshsection
void ULimbExtractorTool::BuildStaticMeshData(
	USkeletalMesh* SkeletalMesh,
	const TArray<int32>& ExtractedVertexIndices,
	TArray<FVector3f>& OutVertices,
	TArray<FVector2D>& OutUVs,
	TArray<FVector>& OutNormals,
	TArray<FColor>& OutColors,
	TArray<FVector>& OutTangents,
	TMap<int32, int32>& OutOldToNewVertexMap,
	TArray<FIntVector>& OutTriangles,
	TArray<int32>& OutMaterialIndices
)
{
	OutVertices.Reset();
	OutUVs.Reset();
	OutNormals.Reset();
	OutColors.Reset();
	OutTangents.Reset();
	OutOldToNewVertexMap.Reset();
	OutMaterialIndices.Reset();
	OutTriangles.Reset();

	const FSkeletalMeshRenderData* RenderData = SkeletalMesh->GetResourceForRendering();
	if (!RenderData || RenderData->LODRenderData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Invalid render data on source mesh."));
		return;
	}

	const FSkeletalMeshLODRenderData& LODData = RenderData->LODRenderData[0];
	const FStaticMeshVertexBuffers& Buffers = LODData.StaticVertexBuffers;
	const FRawStaticIndexBuffer16or32Interface* IndexBuffer = LODData.MultiSizeIndexContainer.GetIndexBuffer();

	// Vertex extraction
	for (int32 OldIndex : ExtractedVertexIndices)
	{
		int32 BuffNum = Buffers.PositionVertexBuffer.GetNumVertices();
		if (OldIndex < 0 || OldIndex >= BuffNum)	continue;

		int32 NewIndex = OutVertices.Num();
		OutOldToNewVertexMap.Add(OldIndex, NewIndex);

		OutVertices.Add(Buffers.PositionVertexBuffer.VertexPosition(OldIndex));
		OutNormals.Add(FVector(Buffers.StaticMeshVertexBuffer.VertexTangentZ(OldIndex)));
		OutTangents.Add(FVector(Buffers.StaticMeshVertexBuffer.VertexTangentX(OldIndex)));
		OutUVs.Add(FVector2d(Buffers.StaticMeshVertexBuffer.GetVertexUV(OldIndex, 0)));
		OutColors.Add(FColor::White); // You could swap this if there's color data
	}

	// Triangle extraction per section
	for (const FSkelMeshRenderSection& Section : LODData.RenderSections)
	{
		const int32 BaseIndex = Section.BaseIndex;
		const int32 NumTris = Section.NumTriangles;

		for (int32 TriIdx = 0; TriIdx < NumTris; ++TriIdx)
		{
			int32 I0 = IndexBuffer->Get(BaseIndex + TriIdx * 3 + 0);
			int32 I1 = IndexBuffer->Get(BaseIndex + TriIdx * 3 + 1);
			int32 I2 = IndexBuffer->Get(BaseIndex + TriIdx * 3 + 2);

			if (OutOldToNewVertexMap.Contains(I0) && OutOldToNewVertexMap.Contains(I1) && OutOldToNewVertexMap.Contains(I2))
			{
				OutTriangles.Add(FIntVector(
					OutOldToNewVertexMap[I0],
					OutOldToNewVertexMap[I1],
					OutOldToNewVertexMap[I2]
				));

				OutMaterialIndices.Add(Section.MaterialIndex);
			}
		}
	}
}

void ULimbExtractorTool::BuildExtractedMeshData(USkeletalMesh* SkeletalMesh,const TArray<int32>& BoneVertexIndices,TArray<FVector3f>& OutVertices,TArray<FVector2D>& OutUVs,TArray<FVector>& OutNormals,TArray<FColor>& OutColors,TArray<FVector>& OutTangents,TMap<int32, int32>& OutOldToNewVertexMap,TArray<FIntVector>& OutTriangles,TArray<FSkinWeightInfo>& OutSkinWeights,TArray<int32>& OriginalVertexIndices)

{

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
		UE_LOG(LogTemp, Error, TEXT("❌ Invalid render data on source mesh."));
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
void ULimbExtractorTool::BuildTriangleIndices(USkeletalMesh* SkeletalMesh, const TSet<int32>& BoneVertexIndexSet,const TMap<int32, int32>& OldToNewVertexMap,TArray<int32>& OutFinalTriangles, TArray<int32>& OutMaterialIndices)
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
void ULimbExtractorTool::AddMaterialSectionToProceduralMesh(UProceduralMeshComponent* NewMesh,int32 SectionIndex,int32 MaterialIndex,const TArray<int32>& SectionTriangles,const TArray<FVector>& FinalVertices,const TArray<FVector>& FinalNormals,const TArray<FVector2D>& FinalUVs,const TArray<FLinearColor>& FinalColors,const TArray<FProcMeshTangent>& MeshTangents,const TArray<FSkeletalMaterial>& SkeletalMaterials,bool bSkipRemap)
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

// so we can separate out multi meshes and their material mappings 
void ULimbExtractorTool::GenerateMaterialToTrianglesMap(const TArray<int32>& MaterialIndices,const TArray<int32>& FinalTriangles,TMap<int32, TArray<int32>>& OutMaterialToTriangles)
{
	OutMaterialToTriangles.Reset();

	const int32 NumTriangles = FinalTriangles.Num() / 3;
	for (int32 TriangleIdx = 0; TriangleIdx < NumTriangles; ++TriangleIdx)
	{
		if (!MaterialIndices.IsValidIndex(TriangleIdx))
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing MaterialIndex for triangle %d"), TriangleIdx);
			continue;
		}

		int32 I0 = FinalTriangles[TriangleIdx * 3 + 0];
		int32 I1 = FinalTriangles[TriangleIdx * 3 + 1];
		int32 I2 = FinalTriangles[TriangleIdx * 3 + 2];
		int32 MaterialIndex = MaterialIndices[TriangleIdx];

		TArray<int32>& SectionList = OutMaterialToTriangles.FindOrAdd(MaterialIndex);
		SectionList.Add(I0);
		SectionList.Add(I1);
		SectionList.Add(I2);
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

// Takes all our final computed data and generates a standard fan-cap
void ULimbExtractorTool::GenerateCapFanFromLoop(
	const TArray<FVector3f>& Loop,
	const TArray<FVector3f>& FinalVertices,
	TArray<FVector3f>& CapVertices,
	TArray<int32>& CapTriangles,
	TArray<FVector2D>& CapUVs,
	TArray<FVector>& CapNormals
)
{
	if (Loop.Num() < 3) return;

	int32 CapVertStart = CapVertices.Num();

	// Compute center
	FVector3f Center(0);
	for (const FVector3f& V : Loop)
		Center += V;
	Center /= Loop.Num();

	// Newell's method for best-fit normal
	FVector3f Normal(0);
	for (int32 i = 0; i < Loop.Num(); ++i)
	{
		const FVector3f& Current = Loop[i];
		const FVector3f& Next = Loop[(i + 1) % Loop.Num()];
		Normal.X += (Current.Y - Next.Y) * (Current.Z + Next.Z);
		Normal.Y += (Current.Z - Next.Z) * (Current.X + Next.X);
		Normal.Z += (Current.X - Next.X) * (Current.Y + Next.Y);
	}
	Normal = Normal.GetSafeNormal();

	// Add center vertex
	int32 CenterIndex = CapVertices.Add(Center);
	CapUVs.Add(FVector2D(0.5f, 0.5f)); // placeholder
	CapNormals.Add(FVector(Normal));

	// Add outer ring
	TArray<int32> RingIndices;
	for (const FVector3f& Pos : Loop)
	{
		RingIndices.Add(CapVertices.Add(Pos));
		CapUVs.Add(FVector2D(0.5f, 0.5f)); // placeholder
		CapNormals.Add(FVector(Normal));
	}

	// Build triangle fan
	for (int32 i = 0; i < RingIndices.Num(); ++i)
	{
		int32 A = RingIndices[i];
		int32 B = RingIndices[(i + 1) % RingIndices.Num()];
		/*CapTriangles.Add(CenterIndex);
		CapTriangles.Add(B); // flipped winding
		CapTriangles.Add(A);*/
		CapTriangles.Add(CenterIndex);
		CapTriangles.Add(A);
		CapTriangles.Add(B);

	}

	int32 CapVertEnd = CapVertices.Num();
	int32 CapVertCount = CapVertEnd - CapVertStart;

	// ✅ Apply UV projection using scoped values
	ProjectPlanarUVsForCap(
		CapVertices,
		CapVertStart,
		CapVertCount,
		Center,
		Normal,
		CapUVs,
		FVector2D(BP_UVScale, BP_UVScale),
		BP_UVRotationDegrees,
		BP_UVOffset
	);

}
//  generates an adjustable gore cap off an edge loop
void ULimbExtractorTool::GenerateDomedCapFromLoop(const TArray<FVector3f>& LoopVerts,TArray<FVector3f>& OutVertices,TArray<int32>& OutTriangles,TArray<FVector2D>& OutUVs,TArray<FVector>& OutNormals,float DomeHeight,float NoiseAmount,int32 RingCount,bool bApplyNoise)
{
	
if (LoopVerts.Num() < 3) return;

int32 DomeVertStart = OutVertices.Num();

const int32 BaseIndex = OutVertices.Num();
const int32 LoopSize = LoopVerts.Num();

// Compute center of loop
FVector3f Center(0);
for (const FVector3f& V : LoopVerts) Center += V;
Center /= LoopVerts.Num();

// Estimate loop normal with Newell's method
FVector3f Normal(0);

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
		FVector3f Direction = (Center - Outer);
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


// ... generate dome verts ...

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
	OutTriangles.Add(B); 
}


int32 DomeVertEnd = OutVertices.Num();
int32 DomeVertCount = DomeVertEnd - DomeVertStart;

	ProjectPlanarUVsForCap(OutVertices, DomeVertStart, DomeVertCount, DomeCenter,  Normal , OutUVs, FVector2D(BP_UVScale, BP_UVScale), BP_UVRotationDegrees, BP_UVOffset);

	UE_LOG(LogTemp, Warning, TEXT("array vals are - OutTriangles %d, OutVertices %d"), OutTriangles.Num(), OutVertices.Num());
}

// creates a 'fadering' which is basically an adjustable length mesh that fills in any rough gaps, to give us a smooth clean edgeloop to build a gore-cap on 
void ULimbExtractorTool::GenerateFadeRingCapFromLoopInfo(
	const FEdgeLoopInfo& LoopInfo,
	TArray<FVector3f>& OutVertices,
	TArray<int32>& OutTriangles,
	TArray<FVector2D>& OutUVs,
	TArray<FVector>& OutNormals,
	TArray<int32>& OutFadeRingIndices,
	TArray<int32>& OutFadeRingTriangles
)
{
	const TArray<FVector3f>& BaseLoopPositions = LoopInfo.Positions;
	const TArray<int32>& VertexIndices = LoopInfo.VertexIndices;

	if (BaseLoopPositions.Num() < 3) return;

	const int32 LoopSize = BaseLoopPositions.Num();
	OutFadeRingIndices.Reset();

	// Step 1: Compute loop center and normal
	FVector3f Center(0);
	for (const FVector3f& V : BaseLoopPositions)
		Center += V;
	Center /= LoopSize;

	FVector3f Normal = ComputeLoopNormalWithTolerance(BaseLoopPositions, 1.5f);
	FVector3f P0 = BaseLoopPositions[0];
	FVector3f P1 = BaseLoopPositions[LoopSize / 2];
	FVector3f LimbAxis = (P1 - P0).GetSafeNormal();
	FVector3f Tangent = (BaseLoopPositions[1] - P0).GetSafeNormal();
	FVector3f SliceNormal = FVector3f::CrossProduct(LimbAxis, Tangent).GetSafeNormal() * -1;

	// Step 2: Estimate average radius
	float AvgRadius = 0.f;
	for (const FVector3f& V : BaseLoopPositions)
		AvgRadius += FVector3f::Dist(V, Center);
	AvgRadius /= LoopSize;
	float Radius = AvgRadius * 0.95f;

	// Step 3: Add base ring verts
	TArray<int32> BaseRing;
	TArray<FVector2D> BaseUVs;

	for (int32 i = 0; i < LoopSize; ++i)
	{
		int32 Idx = OutVertices.Add(BaseLoopPositions[i]);

		int32 GlobalIndex = VertexIndices[i];
		FVector2D UV = CachedMeshContext.FinalUVs.IsValidIndex(GlobalIndex)
			? CachedMeshContext.FinalUVs[GlobalIndex]
			: FVector2D(0.5f, 0.5f);

		OutUVs.Add(UV);
		OutNormals.Add(FVector(SliceNormal));
		BaseRing.Add(Idx);
		BaseUVs.Add(UV); // ✅ store for reuse


	}

	// Step 4: Build tangent basis
	FVector3f X = (BaseLoopPositions[0] - Center).GetSafeNormal();
	FVector3f Y = FVector3f::CrossProduct(SliceNormal, X).GetSafeNormal();

	// Step 5: Create fade ring
	for (int32 i = 0; i < LoopSize; ++i)
	{
		float Angle = (float(i) / LoopSize) * 2.f * PI;
		FVector3f Offset = Radius * (FMath::Cos(Angle) * X + FMath::Sin(Angle) * Y);
		FVector3f Pos = Center + Offset;

		if (LoopInfo.bAddNoise)
		{
			float Noise = FMath::FRandRange(-LoopInfo.NoiseAmount, LoopInfo.NoiseAmount);
			Pos += Normal * Noise;
		}

		int32 FadeIdx = OutVertices.Add(Pos);
		if (BaseUVs.IsValidIndex(i))
		{
			OutUVs.Add(BaseUVs[i]);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BaseUVs out of bounds at index %d"), i);
			OutUVs.Add(FVector2D(0.5f, 0.5f)); // fallback
		}

		OutNormals.Add(FVector(SliceNormal));
		OutFadeRingIndices.Add(FadeIdx);
	}


	// Step 6: Push fade ring outward
	FVector3f PushDir = SliceNormal * LoopInfo.FadeRingHieght;
	for (int32 Index : OutFadeRingIndices)
		OutVertices[Index] += PushDir;

	// Step 7: Stitch base ring to fade ring
	
	for (int32 i = 0; i < LoopSize; ++i)
	{
		int32 A = BaseRing[i];
		int32 B = BaseRing[(i + 1) % LoopSize];
		int32 C = OutFadeRingIndices[i];
		int32 D = OutFadeRingIndices[(i + 1) % LoopSize];

		FVector2D UVA = OutUVs.IsValidIndex(A) ? OutUVs[A] : FVector2D(0.5f, 0.5f);
		FVector2D UVB = OutUVs.IsValidIndex(B) ? OutUVs[B] : FVector2D(0.5f, 0.5f);
		FVector2D UVC = OutUVs.IsValidIndex(C) ? OutUVs[C] : FVector2D(0.5f, 0.5f);
		FVector2D UVD = OutUVs.IsValidIndex(D) ? OutUVs[D] : FVector2D(0.5f, 0.5f);
		if (i == 0)
		{
			int32 E = BaseRing[0];       // first base ring vertex
			int32 F = BaseRing[1];       // second base ring vertex
			int32 G = OutFadeRingIndices[0];   // first fade ring vertex
			int32 H = OutFadeRingIndices[1];   // second fade ring vertex

			FVector3f PosE = OutVertices[E];
			FVector3f PosF = OutVertices[F];
			FVector3f PosG = OutVertices[G];
			FVector3f PosH = OutVertices[H];

			FVector2D UV = OutUVs[E];
			FVector Normal2 = OutNormals[E];

			int32 NewE = OutVertices.Add(PosE);
			int32 NewF = OutVertices.Add(PosF);
			int32 NewG = OutVertices.Add(PosG);
			int32 NewH = OutVertices.Add(PosH);

			OutUVs.Add(UV);
			OutUVs.Add(UV);
			OutUVs.Add(UV);
			OutUVs.Add(UV);

			OutNormals.Add(Normal2);
			OutNormals.Add(Normal2);
			OutNormals.Add(Normal2);
			OutNormals.Add(Normal2);

			OutTriangles.Add(NewE);
			OutTriangles.Add(NewH);
			OutTriangles.Add(NewG);

			OutTriangles.Add(NewE);
			OutTriangles.Add(NewF);
			OutTriangles.Add(NewH);

			OutFadeRingTriangles.Append({ NewE, NewH, NewG, NewE, NewF, NewH });

			continue;
		}

		// Regular stitching for non-seam triangles
		float Spread0 = FVector2D::Distance(UVA, UVD) + FVector2D::Distance(UVD, UVC) + FVector2D::Distance(UVC, UVA);
		float Spread1 = FVector2D::Distance(UVA, UVB) + FVector2D::Distance(UVB, UVD) + FVector2D::Distance(UVD, UVA);

		const float MaxUVSpread = 0.2f;

		if (Spread0 < MaxUVSpread)
		{
			OutTriangles.Add(A);
			OutTriangles.Add(D);
			OutTriangles.Add(C);
			OutFadeRingTriangles.Add(A);
			OutFadeRingTriangles.Add(D);
			OutFadeRingTriangles.Add(C);
		}

		if (Spread1 < MaxUVSpread)
		{
			OutTriangles.Add(A);
			OutTriangles.Add(B);
			OutTriangles.Add(D);
			OutFadeRingTriangles.Add(A);
			OutFadeRingTriangles.Add(B);
			OutFadeRingTriangles.Add(D);
		}
	}
	TArray<FIntVector> FadeTris;
	for (int32 i = 0; i < OutFadeRingTriangles.Num(); i += 3)
	{
		FadeTris.Add(FIntVector(OutFadeRingTriangles[i], OutFadeRingTriangles[i + 1], OutFadeRingTriangles[i + 2]));
	}

	UMeshCleanUtils::RemoveDegenerateTriangles(FadeTris);
	UMeshCleanUtils::RemoveDuplicateTriangles(FadeTris);
	UMeshCleanUtils::RemoveNonManifoldTriangles(FadeTris);

	// Rebuild OutFadeRingTriangles
	OutFadeRingTriangles.Reset();
	for (const FIntVector& Tri : FadeTris)
	{
		OutFadeRingTriangles.Add(Tri.X);
		OutFadeRingTriangles.Add(Tri.Y);
		OutFadeRingTriangles.Add(Tri.Z);
	}
		
}

// takes the edgeloop verts, and filters out ones with extreme distances from the others so a normal is less likely to be skewed
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

	// Step 2: Estimate rough normal using Newell’s method
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

// just a checker to see what verts aare sitting next to what.  It helps sometimes checking our edgeloops
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

// logs bone indices from the skeleton, to see whether what we expect to be used is being picked up
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

// Draw debug lines on the edge loops - helps to track down issues stemming from dirty meshes
void ULimbExtractorTool::DrawEdgeLoopsDebug(const TArray<TArray<FVector3f>>& EdgeLoops,const TArray<FVector3f>& Vertices,UWorld* World,const FTransform& MeshTransform,float Duration,float Thickness){
	if (!World) return;

	for (int32 LoopIndex = 0; LoopIndex < EdgeLoops.Num(); ++LoopIndex)
	{
		const TArray<FVector3f>& Loop = EdgeLoops[LoopIndex];

		for (int32 i = 0; i < Loop.Num(); ++i)
		{
			const FVector3f& V0 = Loop[i];
			const FVector3f& V1 = Loop[(i + 1) % Loop.Num()]; // wrap around

			const FVector Idx = FVector(V0) + FVector(0, 0, 5);
			FVector WorldV0 = MeshTransform.TransformPosition(FVector(V0));
			FVector WorldV1 = MeshTransform.TransformPosition(FVector(V1));

			FColor LineColor = FColor::MakeRedToGreenColorFromScalar((float)LoopIndex / (float)EdgeLoops.Num());

			DrawDebugLine(World, WorldV0, WorldV1, LineColor, false, Duration, 0, Thickness);
			DrawDebugPoint(World, WorldV0, 6.0f, LineColor, false, Duration);
			FString num = FString::FromInt(i);
			//SpawnTextLabelInEditor(GetWorld(), Idx, num, FColor::Blue);

		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Drawn %d edge loops for debug"), EdgeLoops.Num());
}

// logs vertex skinweight data... logs are long but it helps to track down whether weighted bones vertices are being picked up
void ULimbExtractorTool::ValidateVertexToSkinWeightMapping(const USkeletalMesh* SourceMesh,const TArray<FVector3f>& OutVertices,const TArray<FSkinWeightInfo>& OutSkinWeights,const TArray<int32>& OriginalVertexIndices)
{
	const FSkeletalMeshRenderData* RenderData = SourceMesh->GetResourceForRendering();
	if (!RenderData || RenderData->LODRenderData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Invalid skeletal render data."));
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
			UE_LOG(LogTemp, Error, TEXT("❌ Missing original index for vertex %d"), i);
			++ErrorCount;
			continue;
		}

		const int32 OrigIdx = OriginalVertexIndices[i];

		// Validate position match
		const FVector3f& Current = OutVertices[i];
		const FVector3f& Original = Positions.VertexPosition(OrigIdx);

		if (!Current.Equals(Original, 0.01f))
		{
			UE_LOG(LogTemp, Warning, TEXT("⚠️ Position mismatch [Vert %d] → %s vs %s"),
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
					UE_LOG(LogTemp, Warning, TEXT("⚠️ Mismatch [Vert %d Influence %d] Bone: %d/%d Weight: %d/%d"),
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
			UE_LOG(LogTemp, Error, TEXT("❌ Missing skin weight data for vertex %d"), i);
			++ErrorCount;
		}
	}

	if (ErrorCount == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("✅ Validation passed — vertex positions and skin weights match original mesh."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Validation failed — %d mismatch(es) detected."), ErrorCount);
	}
}

// logs whether the influence bones we expect to be used are being used
void ULimbExtractorTool::ValidateInfluenceBoneMapping(USkeletalMesh* SourceMesh,const TMap<int32, int32>& BoneRemap,const TArray<FSkinWeightInfo>& SkinWeights)
{
	UE_LOG(LogTemp, Display, TEXT("🧪 Validating influence bones against BoneRemap..."));

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
					UE_LOG(LogTemp, Error, TEXT("❌ Vertex %d influence %d: Bone %d NOT found in BoneRemap!"), i, j, BoneIndex);
				}
			}
		}
	}

	UE_LOG(LogTemp, Display, TEXT("✅ Validation complete — %d unique bones used"), UsedBones.Num());
}


#pragma endregion

#pragma region Cap uv generation
///////// Cap UV section //////////////////

/// This is our main function to take the vertice array and project the uvs given the adjustment params 
void ULimbExtractorTool::ProjectPlanarUVsForCap(const TArray<FVector3f>& Vertices,int32 StartIndex,int32 Count,const FVector3f& PlaneOrigin,const FVector3f& PlaneNormal,TArray<FVector2D>& UVs,FVector2D UVScale,float UVRotationDegrees,FVector2D UVOffset){
	if (StartIndex < 0 || Count <= 0 || (StartIndex + Count) > Vertices.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Invalid vertex range for UV projection: Start=%d Count=%d"), StartIndex, Count);
		return;
	}

	// Ensure UV array is long enough
	if (UVs.Num() < Vertices.Num())
	{
		UVs.SetNum(Vertices.Num());
	}

	// Build a local basis for projection
	FVector3f U = FVector3f::CrossProduct(PlaneNormal, FVector3f::UpVector);
	if (U.SizeSquared() < KINDA_SMALL_NUMBER)
		U = FVector3f::CrossProduct(PlaneNormal, FVector3f::RightVector);
	U.Normalize();

	FVector3f V = FVector3f::CrossProduct(PlaneNormal, U).GetSafeNormal();

	// Rotation
	const float AngleRad = FMath::DegreesToRadians(UVRotationDegrees);
	const float CosA = FMath::Cos(AngleRad);
	const float SinA = FMath::Sin(AngleRad);

	// Project the range
	for (int32 i = 0; i < Count; ++i)
	{
		int32 VIndex = StartIndex + i;
		const FVector3f Local = Vertices[VIndex] - PlaneOrigin;

		const float UCoord = FVector3f::DotProduct(Local, U);
		const float VCoord = FVector3f::DotProduct(Local, V);

		float RotU = UCoord * CosA - VCoord * SinA;
		float RotV = UCoord * SinA + VCoord * CosA;

		UVs[VIndex] = (FVector2D(RotU, RotV) * UVScale) + UVOffset;
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


