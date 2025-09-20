
#include "ULimbExtractorTool.h"

#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine/TextRenderActor.h"

#include <Components/TextRenderComponent.h>

#include "Math/UnrealMathUtility.h"
#include <Rendering/SkeletalMeshLODModel.h>

#include "Engine/SkeletalMesh.h"
#include "Rendering/SkeletalMeshModel.h"

#include "Rendering/SkinWeightVertexBuffer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/Skeleton.h"
#include "Materials/Material.h"
#include "UModalTaskHelper.h"
#include "UMeshMergeUtility.h"

/////////////// Helper functions //////////////////////////////////////////////////////
void ULimbExtractorTool::PrintError(FString string)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *string);
}

void ULimbExtractorTool::SetMinimumWeightThreshold(int32 value)
{
	MinimumWeight = value;  // the only place this is called is the menu, which is currently preset at 255
}

void ULimbExtractorTool::SetExclusionBones(TArray<FName> Exclusions)
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

AActor* ULimbExtractorTool::GetOwnerActor()
{
	return OwnerActor;
}

#pragma region Edgeloop clustering experiment 

// NEW: Main clustering processing function
void ULimbExtractorTool::ProcessEdgeLoopsWithClustering(TArray<FEdgeLoopInfo>& EdgeLoopInfoArray)
{
	if (EdgeLoopInfoArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No edge loops to cluster"));
		return;
	}

	// Convert FEdgeLoopInfo to FEdgeLoop for clustering
	TArray<FEdgeLoop> RawLoops;
	TMap<int32, int32> LoopIndexMap; // Maps clustered loop index to original EdgeLoopInfo index

	for (int32 i = 0; i < EdgeLoopInfoArray.Num(); ++i)
	{
		FEdgeLoop ConvertedLoop = ConvertEdgeLoopInfoToEdgeLoop(EdgeLoopInfoArray[i]);
		RawLoops.Add(ConvertedLoop);
		LoopIndexMap.Add(RawLoops.Num() - 1, i);
	}

	UE_LOG(LogTemp, Log, TEXT("Starting clustering with %d raw loops"), RawLoops.Num());

	// Step 1: Filter loops if enabled
	TArray<FEdgeLoop> FilteredLoops = RawLoops;
	if (bFilterSmallFragments)
	{
		FilteredLoops = UEdgeLoopClusteringUtility::FilterLoops(RawLoops, MinLoopArea, 4);
		UE_LOG(LogTemp, Log, TEXT("After filtering: %d loops (removed %d small fragments)"),
			FilteredLoops.Num(), RawLoops.Num() - FilteredLoops.Num());
	}

	// Step 2: Cluster nearby loops
	TArray<FEdgeLoopCluster> Clusters = UEdgeLoopClusteringUtility::ClusterEdgeLoops(FilteredLoops, ClusterRadius);
	DetectedClusters = Clusters; // Store for debugging/inspection

	UE_LOG(LogTemp, Log, TEXT("Created %d clusters from %d loops"), Clusters.Num(), FilteredLoops.Num());

	// Step 3: Process clusters and update EdgeLoopInfoArray
	if (bMergeOverlappingLoops)
	{
		UpdateEdgeLoopInfosFromClusters(Clusters);
		UE_LOG(LogTemp, Log, TEXT("Merged overlapping loops. Final count: %d"), EdgeLoopInfos.Num());
	}
	else
	{
		// Just update with cluster information but don't merge
		for (int32 ClusterIdx = 0; ClusterIdx < Clusters.Num(); ++ClusterIdx)
		{
			const FEdgeLoopCluster& Cluster = Clusters[ClusterIdx];
			for (int32 LoopIdx = 0; LoopIdx < Cluster.Loops.Num(); ++LoopIdx)
			{
				// Find corresponding EdgeLoopInfo and mark it with cluster info
				// This is a simplified approach - you might want more sophisticated mapping
				if (EdgeLoopInfoArray.IsValidIndex(ClusterIdx))
				{
					// Add cluster information to your EdgeLoopInfo structure if needed
					// EdgeLoopInfoArray[ClusterIdx].ClusterID = ClusterIdx;
				}
			}
		}
	}
}

// NEW: Convert FEdgeLoopInfo to FEdgeLoop
FEdgeLoop ULimbExtractorTool::ConvertEdgeLoopInfoToEdgeLoop(const FEdgeLoopInfo& LoopInfo)
{
	FEdgeLoop EdgeLoop;

	// Convert positions from FVector3f to FVector if needed
	EdgeLoop.Vertices.Reserve(LoopInfo.Positions.Num());
	for (const FVector3f& Pos : LoopInfo.Positions)
	{
		EdgeLoop.Vertices.Add(FVector(Pos));
	}

	// Set additional properties
	EdgeLoop.OriginalMeshSection = LoopInfo.VotedMaterialIndex;

	// Calculate properties
	EdgeLoop.CalculateProperties();

	return EdgeLoop;
}

// NEW: Convert FEdgeLoop back to FEdgeLoopInfo
FEdgeLoopInfo ULimbExtractorTool::ConvertEdgeLoopToEdgeLoopInfo(const FEdgeLoop& EdgeLoop, int32 OriginalLoopNumber)
{
	FEdgeLoopInfo LoopInfo;

	// Convert vertices back to FVector3f
	LoopInfo.Positions.Reserve(EdgeLoop.Vertices.Num());
	for (const FVector& Vertex : EdgeLoop.Vertices)
	{
		LoopInfo.Positions.Add(FVector3f(Vertex));
	}

	// Set material index from original mesh section
	LoopInfo.VotedMaterialIndex = EdgeLoop.OriginalMeshSection;
	LoopInfo.LoopNumber = OriginalLoopNumber >= 0 ? OriginalLoopNumber : 0;

	// Note: VertexIndices and UVs would need to be reconstructed or preserved separately
	// This is a limitation when converting back - you might want to preserve this data

	return LoopInfo;
}

// NEW: Update EdgeLoopInfos from clustered results
void ULimbExtractorTool::UpdateEdgeLoopInfosFromClusters(const TArray<FEdgeLoopCluster>& Clusters)
{
	TArray<FEdgeLoopInfo> NewEdgeLoopInfos;
	NewEdgeLoopInfos.Reserve(Clusters.Num());

	for (int32 ClusterIdx = 0; ClusterIdx < Clusters.Num(); ++ClusterIdx)
	{
		const FEdgeLoopCluster& Cluster = Clusters[ClusterIdx];

		if (Cluster.Loops.Num() == 1)
		{
			// Single loop cluster - convert back as-is
			FEdgeLoopInfo NewInfo = ConvertEdgeLoopToEdgeLoopInfo(Cluster.Loops[0], ClusterIdx);
			NewEdgeLoopInfos.Add(NewInfo);

			UE_LOG(LogTemp, Log, TEXT("Cluster %d: Single loop with %d vertices"),
				ClusterIdx, Cluster.Loops[0].Vertices.Num());
		}
		else if (Cluster.Loops.Num() > 1)
		{
			// Multi-loop cluster - merge them
			FEdgeLoop MergedLoop = UEdgeLoopClusteringUtility::MergeClusterLoops(Cluster);

			if (MergedLoop.Vertices.Num() >= 4) // Ensure we have a valid loop
			{
				FEdgeLoopInfo MergedInfo = ConvertEdgeLoopToEdgeLoopInfo(MergedLoop, ClusterIdx);
				NewEdgeLoopInfos.Add(MergedInfo);

				UE_LOG(LogTemp, Log, TEXT("Cluster %d: Merged %d loops into 1 loop with %d vertices"),
					ClusterIdx, Cluster.Loops.Num(), MergedLoop.Vertices.Num());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Cluster %d: Merged loop too small (%d vertices), skipping"),
					ClusterIdx, MergedLoop.Vertices.Num());
			}
		}
	}

	// Replace the original EdgeLoopInfoArray with clustered results
	EdgeLoopInfos = NewEdgeLoopInfos;
}

// Optional: Debug function to visualize clusters
UFUNCTION(CallInEditor = true, Category = "Debug")
void ULimbExtractorTool::DebugDrawClusters()
{
	if (!GEngine || DetectedClusters.Num() == 0)
	{
		return;
	}

	// Clear previous debug draws
	FlushPersistentDebugLines(GEngine->GetWorld());

	// Draw each cluster in a different color
	TArray<FColor> ClusterColors = {
		FColor::Red, FColor::Green, FColor::Blue, FColor::Yellow,
		FColor::Magenta, FColor::Cyan, FColor::Orange, FColor::Purple
	};

	for (int32 ClusterIdx = 0; ClusterIdx < DetectedClusters.Num(); ++ClusterIdx)
	{
		const FEdgeLoopCluster& Cluster = DetectedClusters[ClusterIdx];
		FColor ClusterColor = ClusterColors[ClusterIdx % ClusterColors.Num()];

		// Draw cluster centroid
		DrawDebugSphere(GEngine->GetWorld(), Cluster.ClusterCentroid, 2.0f, 8, ClusterColor, true, 10.0f);

		// Draw cluster radius
		DrawDebugSphere(GEngine->GetWorld(), Cluster.ClusterCentroid, Cluster.ClusterRadius, 16, ClusterColor, true, 10.0f, 0, 0.5f);

		// Draw each loop in the cluster
		for (int32 LoopIdx = 0; LoopIdx < Cluster.Loops.Num(); ++LoopIdx)
		{
			const FEdgeLoop& Loop = Cluster.Loops[LoopIdx];

			// Draw loop vertices
			for (int32 VertIdx = 0; VertIdx < Loop.Vertices.Num(); ++VertIdx)
			{
				int32 NextVertIdx = (VertIdx + 1) % Loop.Vertices.Num();
				DrawDebugLine(GEngine->GetWorld(), Loop.Vertices[VertIdx], Loop.Vertices[NextVertIdx],
					ClusterColor, true, 10.0f, 0, LoopIdx == Cluster.DominantLoopIndex ? 3.0f : 1.0f);
			}

			// Draw loop centroid
			DrawDebugSphere(GEngine->GetWorld(), Loop.Centroid, 1.0f, 6,
				LoopIdx == Cluster.DominantLoopIndex ? FColor::White : ClusterColor, true, 10.0f);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Debug drew %d clusters"), DetectedClusters.Num());
}

// NEW: Helper function to find original EdgeLoopInfo from clustered EdgeLoop
FEdgeLoopInfo* ULimbExtractorTool::FindOriginalEdgeLoopInfo(const FEdgeLoop& EdgeLoop, TArray<FEdgeLoopInfo>& EdgeLoopInfoArray)
{
	// Try to match based on approximate vertex count and centroid position
	FVector EdgeLoopCentroid = EdgeLoop.Centroid;
	int32 EdgeLoopVertexCount = EdgeLoop.Vertices.Num();

	float BestMatch = FLT_MAX;
	FEdgeLoopInfo* BestInfo = nullptr;

	for (FEdgeLoopInfo& Info : EdgeLoopInfoArray)
	{
		if (Info.Positions.Num() == 0) continue;

		// Calculate centroid of EdgeLoopInfo
		FVector InfoCentroid = FVector::ZeroVector;
		for (const FVector3f& Pos : Info.Positions)
		{
			InfoCentroid += FVector(Pos);
		}
		InfoCentroid /= Info.Positions.Num();

		// Calculate match score (lower is better)
		float CentroidDistance = FVector::Dist(EdgeLoopCentroid, InfoCentroid);
		int32 VertexCountDiff = FMath::Abs(EdgeLoopVertexCount - Info.Positions.Num());
		float MatchScore = CentroidDistance + (VertexCountDiff * 0.1f); // Weight vertex count difference less

		if (MatchScore < BestMatch && CentroidDistance < 1.0f) // Within 1 unit distance
		{
			BestMatch = MatchScore;
			BestInfo = &Info;
		}
	}

	return BestInfo;
}

#pragma endregion

//////////////////// Main functions for the extraction process ////////////////////////////////////////////////////////////////////
// The only called function in BP - does all the mesh generating and capping
void ULimbExtractorTool::ExtractBoneToMesh(USkeletalMesh* SkeletalMesh, FName BoneName, bool CreateSkelMesh, bool CreateBoneChain)
{
	// precautionary deletion of actors, so we dont get level actors with the same name caused by calling the CreateSkelMesh, then CreateStaticMesh
	OnEditorUtilityClosed();
	if (SkeletalMesh)
	{
		// set our mesh for later
		InMesh = SkeletalMesh;

		if (CreateSkelMesh)
		{
			CreateSkeletalMesh(SkeletalMesh, BoneName, CreateBoneChain);

		}
		else
		{
			CreateStaticMesh(SkeletalMesh, BoneName, CreateBoneChain);
		}
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

	//UE_LOG(LogTemp, Warning, TEXT("BoneHierachy length - %d"), BoneHierachy.Num());

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
	if (RevCapActors.Num() <= 0)
	{
		for (auto& actor : RevCapActors)
		{
			actor.Value->Destroy();
		}
		ReverseCapComponents.Empty();
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
		OwnerActor->SetActorLabel(FString::Printf(TEXT("LimbProcMeshPreviewActor_%d"), PreviewActorCounter++));

	}
	if(OwnerActor)
	{
		PreviewMesh = NewObject<UProceduralMeshComponent>(OwnerActor);
		PreviewMesh->RegisterComponent();
		PreviewMesh->PrimaryComponentTick.bCanEverTick = true;
		PreviewMesh->SetComponentTickEnabled(true);
		PreviewMesh->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		/*if (PreviewMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("PreviewMesh is valid"));

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PreviewMesh is null"));

		}*/
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
		OwnerActor->SetActorLabel(FString::Printf(TEXT("LimbSkelMeshPreviewActor_%d"), PreviewActorCounter++));

	}
	SkelMesh = NewObject<USkeletalMeshComponent>(OwnerActor);
	if (OutLimbSkelMesh && !OutLimbSkelMesh->GetResourceForRendering()->LODRenderData.IsEmpty())
	{
		SkelMesh->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		//AutoSkinMeshComponent(SkelMesh, OutLimbSkelMesh);

		// Register and attach properly
		SkelMesh->RegisterComponent();

		OwnerActor->AddInstanceComponent(SkelMesh);
		SkelMesh->SetComponentTickEnabled(true);
		//SkelMesh->TickComponent(0.f, ELevelTick::LEVELTICK_All, nullptr);
		SkelMesh->PrimaryComponentTick.bCanEverTick = true;
		SkelMesh->SetComponentTickEnabled(true);

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

// spawns the procedural mesh and creates a PreviewActor member
void ULimbExtractorTool::SpawnRevCapActor(int32 index)
{
	// Clean up existing actor if present
	if (RevCapActors.Contains(index))
	{
		if (IsValid(RevCapActors[index]))
		{
			RevCapActors[index]->Destroy();
		}
		RevCapActors.Remove(index);
	}

	// Also clean up the component reference since the actor is being destroyed
	if (ReverseCapComponents.Contains(index))
	{
		ReverseCapComponents.Remove(index);
	}

	// Spawn new actor
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("No valid world for spawning RevCap actor"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	static int32 PreviewActorCounter = 0;
	SpawnParams.Name = FName(*FString::Printf(TEXT("LimbRevCap_%d"), PreviewActorCounter++));
	SpawnParams.ObjectFlags |= RF_Transient;

	AActor* NewRevCapActor = World->SpawnActor<AActor>(AActor::StaticClass(), SpawnParams);
	if (NewRevCapActor)
	{
		NewRevCapActor->SetActorLabel(FString::Printf(TEXT("LimbMeshPreviewActor_LimbRevCap_%d"), index));

		// Store the new actor
		RevCapActors.Add(index, NewRevCapActor);

		// Attach to main owner actor if needed
		if (OwnerActor)
		{
			NewRevCapActor->AttachToActor(OwnerActor, FAttachmentTransformRules::KeepWorldTransform);
		}

		UE_LOG(LogTemp, Log, TEXT("Spawned new RevCap actor for index %d"), index);
	}
}

void ULimbExtractorTool::CreateReverseCapPreview(int32 index)
{
	UUMeshMergeUtility* Util = NewObject<UUMeshMergeUtility>();
	if (!Util)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Failed to create MeshMergeUtility."));
		return;
	}
	if (!ReverseCapComponents.Contains(index)) return;
	{
		UE_LOG(LogTemp, Error, TEXT("❌ No ReverseCapComponent found for index %d."), index);
		return;
	}
	if (ReverseCapComponents[index]->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("Create Reverse cap has valid cap to copy"));

		Util->BuildStaticMeshFromReverseCaps(index, ReverseCapComponents);
	}
	// Mark for cleanup
	Util->MarkAsGarbage();
}

void ULimbExtractorTool::MergeCapsWithMesh(bool StaticMesh, bool WeldMeshes)
{
	
	// Still needs the progress bar updated yt
	UModalTaskHelper::ShowModalWhileRunning(TEXT("Merging caps with mesh..."), 2);

	UUMeshMergeUtility* Util = NewObject<UUMeshMergeUtility>();
	if (!Util)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Failed to create MeshMergeUtility."));
		return;
	}

	if (StaticMesh)
	{
		// prompt the user if we dont have a valid input procedural mesh
		UModalTaskHelper::RunWithConfirmationIf(!PreviewMesh, TEXT("ProceduralMeshComponent Input Check"), TEXT("\n Please generate a preview mesh first - aborting mesh build"), [&]() {return; });
		// we return if theres no mesh input, or it could crash because we need this mesh to draw data from
		if (!PreviewMesh)
		{
			return;
		}
		// only run this if we have a mesh input
		TArray<int32> SortedKeys;
		CapComponents.GetKeys(SortedKeys);
		SortedKeys.Sort(); 
		// fill CapMeshArray 
		TArray<UProceduralMeshComponent*> CapMeshArray;
		for (int32 Key : SortedKeys)
		{
			if (UProceduralMeshComponent* CapComp = CapComponents.FindRef(Key))
			{
				CapMeshArray.Add(CapComp);

			}
		}
		// prompt the user that the mesh wont have caps and will have open holes from no cap data
		UModalTaskHelper::RunWithConfirmationIf(CapMeshArray.Num() == 0,TEXT("Caps Check Failed"),TEXT("CapComponents Data is empty, as no caps have been generated.\nThe generated Static Mesh will have open holes."),[&](){return;});
		// we run this anyway, because data loss wont crash the building
		Util->MergeMultipleCapsIntoProceduralMesh(CapMeshArray, CachedMeshContext, PreviewMesh, CapSkinningInfos, true);
	}
	else
	{
		// prompt the user if theres no inputmesh - i.e generating a mesh first
		UModalTaskHelper::RunWithConfirmationIf(!OutLimbSkelMesh, TEXT("SkeletalMesh Input Check"), TEXT("\n Please generate a skeletal mesh first - aborting mesh build"), [&]() {return; });
		// if no mesh then we have to return or it could crash because its needed to draw base-mesh data from
		if (!OutLimbSkelMesh)
		{
			return;
		}
		// check for valid caps and skinning infos, then fill the CapMeshArray
		TArray<UProceduralMeshComponent*> CapMeshArray;

		if (CapComponents.Num() > 0 && CapSkinningInfos.Num() > 0)
		{
			TArray<int32> SortedKeys;
			CapComponents.GetKeys(SortedKeys);
			SortedKeys.Sort(); // Optional, for consistent ordering

			for (int32 Key : SortedKeys)
			{
				if (UProceduralMeshComponent* CapComp = CapComponents.FindRef(Key))
				{
					CapMeshArray.Add(CapComp);
				}
			}
			// Merge the cap into the skeletal mesh
			Util->MergeMultipleCapsIntoSkeletalMesh(CapMeshArray, RawVertexData, OutLimbSkelMesh, CapSkinningInfos, WeldMeshes);

		}
			
		else  // theres not enough data, so prompt the user and return
		{
			bool bUserConfirmed = UModalTaskHelper::ShowBlockingConfirmation(TEXT("Capping Data is Invalid"),TEXT("CapComponents or Skinning Data is empty,  generated mesh will have open edgeloops"));

			if (bUserConfirmed)
			{
				// Merge the cap into the skeletal mesh
				UE_LOG(LogTemp, Warning, TEXT("running merge multiple caps"))

				Util->MergeMultipleCapsIntoSkeletalMesh(CapMeshArray, RawVertexData, OutLimbSkelMesh, CapSkinningInfos, WeldMeshes);
			}
		}
	}

		// Mark for cleanup
		Util->MarkAsGarbage();
	
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
						//UE_LOG(LogTemp, Warning, TEXT("Invalid compact index %d at section %d"), CompactBoneIndex, SectionIndex);
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
	
	//UE_LOG(LogTemp, Log, TEXT("Extracted %d vertices for bone '%s' (index %d)"), OutVertices.Num(), *BoneName.ToString(), TargetBoneIndex);

}

// Only extract vertices weighted to bones in your chain, and only store weights for chain bones
void ULimbExtractorTool::ExtractSkelMeshVertices(USkeletalMesh* SkeletalMesh, FName BoneName, TArray<FRawVertexData>& VertexData, float MinimumWeightThreshold, const TArray<int32>& ChainBoneIndices, TSet<int32> ProcessedVertices)
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
	const FReferenceSkeleton& RefSkeleton = SkeletalMesh->GetRefSkeleton();
	const int32 TargetBoneIndex = RefSkeleton.FindBoneIndex(BoneName);

	if (TargetBoneIndex == INDEX_NONE)
	{
		//UE_LOG(LogTemp, Error, TEXT("Bone %s not found."), *BoneName.ToString());
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
			Data.RemappedIndex = VertexData.Num();

			bool bHasValidInfluence = false;

			for (int32 InfluenceIndex = 0; InfluenceIndex < MAX_TOTAL_INFLUENCES; ++InfluenceIndex)
			{
				const uint8 CompactBoneIndex = SkinWeight.InfluenceBones[InfluenceIndex];
				const uint8 WeightByte = SkinWeight.InfluenceWeights[InfluenceIndex];

				if (WeightByte == 0 || !BoneMap.IsValidIndex(CompactBoneIndex)) continue;

				const int32 SkeletonBoneIndex = BoneMap[CompactBoneIndex];
				const float Weight = static_cast<float>(WeightByte) / 255.0f;

				if (SkeletonBoneIndex == TargetBoneIndex && Weight >= EffectiveThreshold)
				{
					bHasValidInfluence = true;
				}

				// Store bone NAME instead of index
				FName SkeletonBoneName = RefSkeleton.GetBoneName(SkeletonBoneIndex);
				Data.BoneNames.Add(SkeletonBoneName);
				Data.BoneWeights.Add(Weight);
				Data.BoneInfluences.Add(SkeletonBoneIndex); // Store the skeleton bone index			}
			}
			if (bHasValidInfluence && !ProcessedVertices.Contains(VertexIndex))
			{
				VertexData.Add(Data);
				ProcessedVertices.Add(VertexIndex); // Mark this vertex as processed

			}
		}
		VertexBase += NumVertices;
	}
	//UE_LOG(LogTemp, Log, TEXT("Extracted %d vertices for bone '%s' (index %d)"), VertexData.Num(), *BoneName.ToString(), TargetBoneIndex);
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

	int32 NewRootIndex = 0; // usually first bone in sorted array
	int32 OldRootIndex = SortedBoneIndices[0];
	FTransform Accumulated = GetAccumulatedTransform(SkeletalMesh->GetRefSkeleton(), OldRootIndex);
	RefSkelModifier.UpdateRefPoseTransform(NewRootIndex, Accumulated);

}

// Used in the CreateSkeletalLimbData function for accumulating the parent bone bindpose transforms and ensuring the new root bone has the correct mesh position
FTransform ULimbExtractorTool::GetAccumulatedTransform(const FReferenceSkeleton& RefSkeleton, int32 BoneIndex)
{
	FTransform Accumulated = FTransform::Identity;
	int32 i = RefSkeleton.GetParentIndex(BoneIndex);
	
	while (RefSkeleton.IsValidIndex(BoneIndex))
	{
		Accumulated *= RefSkeleton.GetRefBonePose()[BoneIndex];
		BoneIndex = RefSkeleton.GetParentIndex(BoneIndex);
	}
	return Accumulated;
}

// This uses our colleced per-vertex data, and feeds it to the generated skeletal mesh for skinning, as well as using the buffers to create the mesh
void ULimbExtractorTool::CreateLimbMeshFromRawVertexData(
	USkeletalMesh* SourceMesh,
	USkeletalMesh*& OutLimbMesh,
	const TArray<FVector3f>& Vertices,
	const TArray<FVector2D>& UVs,
	const TArray<FVector>& Normals,
	const TArray<FColor>& Colors,
	const TArray<FVector>& Tangents,
	const TArray<FIntVector>& Triangles,
	TArray<FRawVertexData> VertexData,
	const TMap<int32, int32>& BoneRemap,
	const TSet<int32>& SkinWeightBoneIndicesUsed,
	TArray<int32> MaterialIndices)
{
#if WITH_EDITORONLY_DATA
	// Validate input data
	if (!SourceMesh || !OutLimbMesh || Vertices.Num() == 0 || Triangles.Num() == 0 || VertexData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Invalid mesh input - V: %d, T: %d, VD: %d"),
			Vertices.Num(), Triangles.Num(), VertexData.Num());
		return;
	}

	if (Vertices.Num() != VertexData.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Vertex count mismatch - Vertices: %d, VertexData: %d"),
			Vertices.Num(), VertexData.Num());
		return;
	}

	// Get mesh model and clear existing LOD data
	FSkeletalMeshModel* MeshModel = OutLimbMesh->GetImportedModel();
	if (!MeshModel)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Failed to get mesh model"));
		return;
	}

	MeshModel->LODModels.Empty();
	FSkeletalMeshLODModel* LODModel = new FSkeletalMeshLODModel();
	MeshModel->LODModels.Add(LODModel);

	// Get reference skeletons
	const FReferenceSkeleton& SourceRefSkeleton = SourceMesh->GetRefSkeleton();
	const FReferenceSkeleton& NewRefSkeleton = OutLimbMesh->GetRefSkeleton();

	// Collect all unique bone names that are actually used
	TSet<FName> BoneNamesInUse;
	for (const FRawVertexData& RawData : VertexData)
	{
		for (const FName& BoneName : RawData.BoneNames)
		{
			if (BoneName != NAME_None)
			{
				BoneNamesInUse.Add(BoneName);
			}
		}
	}

	// Create bone mapping from names to target skeleton indices
	TArray<uint16> GlobalBoneMap;
	TMap<FName, int32> BoneNameToCompactIndex;

	for (const FName& BoneName : BoneNamesInUse)
	{
		int32 TargetBoneIndex = NewRefSkeleton.FindBoneIndex(BoneName);
		if (TargetBoneIndex != INDEX_NONE)
		{
			int32 CompactIndex = GlobalBoneMap.AddUnique(TargetBoneIndex);
			BoneNameToCompactIndex.Add(BoneName, CompactIndex);
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("⚠️ Bone '%s' not found in target skeleton"), *BoneName.ToString());
		}
	}

	GlobalBoneMap.Sort(); // Ensure consistent ordering

	// Update required bones for LOD
	for (int32 BoneIdx : GlobalBoneMap)
	{
		if (NewRefSkeleton.IsValidIndex(BoneIdx))
		{
			LODModel->RequiredBones.AddUnique(BoneIdx);
			LODModel->ActiveBoneIndices.AddUnique(BoneIdx);
		}
	}

	// Rebuild bone name to compact index mapping after sorting
	BoneNameToCompactIndex.Empty();
	for (const FName& BoneName : BoneNamesInUse)
	{
		int32 TargetBoneIndex = NewRefSkeleton.FindBoneIndex(BoneName);
		if (TargetBoneIndex != INDEX_NONE)
		{
			int32 CompactIndex = GlobalBoneMap.Find(TargetBoneIndex);
			if (CompactIndex != INDEX_NONE)
			{
				BoneNameToCompactIndex.Add(BoneName, CompactIndex);
			}
		}
	}

	// Create all soft skin vertices
	TArray<FSoftSkinVertex> AllVertices;
	AllVertices.SetNum(Vertices.Num());

	for (int32 i = 0; i < Vertices.Num(); ++i)
	{
		const FRawVertexData& RawData = VertexData[i];
		FSoftSkinVertex& V = AllVertices[i];

		// Set basic vertex data
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

		// Process bone weights
		int32 ValidInfluences = 0;
		float TotalWeight = 0.0f;

		int32 BoneNameCount = FMath::Min(RawData.BoneNames.Num(), MAX_TOTAL_INFLUENCES);
		int32 WeightCount = FMath::Min(RawData.BoneWeights.Num(), MAX_TOTAL_INFLUENCES);
		int32 ProcessCount = FMath::Min(BoneNameCount, WeightCount);

		for (int32 j = 0; j < ProcessCount; ++j)
		{
			const FName& BoneName = RawData.BoneNames[j];
			const float Weight = RawData.BoneWeights[j];

			if (Weight <= 0.0f || BoneName == NAME_None) continue;

			const int32* CompactIndex = BoneNameToCompactIndex.Find(BoneName);
			if (!CompactIndex)
			{
				continue; // Skip bones not in our target skeleton
			}

			V.InfluenceBones[ValidInfluences] = static_cast<uint8>(*CompactIndex);
			V.InfluenceWeights[ValidInfluences] = FMath::Clamp(FMath::RoundToInt(Weight * 255.0f), 0, 255);
			TotalWeight += V.InfluenceWeights[ValidInfluences];
			ValidInfluences++;
		}

		// Handle fallback and normalization
		if (ValidInfluences == 0 || TotalWeight == 0)
		{
			V.InfluenceBones[0] = 0;
			V.InfluenceWeights[0] = 255;
		}
		else if (FMath::Abs(TotalWeight - 255.0f) > 1.0f)
		{
			// Normalize weights
			float Scale = 255.0f / TotalWeight;
			int32 NewTotal = 0;

			for (int32 j = 0; j < ValidInfluences; ++j)
			{
				V.InfluenceWeights[j] = FMath::Clamp(FMath::RoundToInt(V.InfluenceWeights[j] * Scale), 1, 255);
				NewTotal += V.InfluenceWeights[j];
			}

			// Fix rounding errors
			if (NewTotal != 255 && ValidInfluences > 0)
			{
				int32 Diff = 255 - NewTotal;
				V.InfluenceWeights[0] = FMath::Clamp(V.InfluenceWeights[0] + Diff, 1, 255);
			}
		}
	}

	// Create material to triangles mapping
	TMap<int32, TArray<int32>> MaterialToTriangles;
	TArray<int32> FinalTriangles;
	for (const FIntVector& Vec : Triangles)
	{
		FinalTriangles.Add(Vec.X);
		FinalTriangles.Add(Vec.Y);
		FinalTriangles.Add(Vec.Z);
	}

	// Generate material mapping
	GenerateMaterialToTrianglesMap(MaterialIndices, FinalTriangles, MaterialToTriangles);

	// Create sections for each material
	LODModel->NumVertices = 0;
	int32 BaseIndex = 0;

	for (const TPair<int32, TArray<int32>>& Pair : MaterialToTriangles)
	{
		int32 MaterialIndex = Pair.Key;
		const TArray<int32>& TriangleIndices = Pair.Value;

		if (TriangleIndices.Num() % 3 != 0)
		{
			UE_LOG(LogTemp, Warning, TEXT(" Material %d has invalid triangle count: %d"),
				MaterialIndex, TriangleIndices.Num());
			continue;
		}

		// Collect unique vertices for this section
		TSet<int32> UsedVertexIndicesSet;
		for (int32 TriIdx : TriangleIndices)
		{
			if (TriIdx >= 0 && TriIdx < AllVertices.Num())
			{
				UsedVertexIndicesSet.Add(TriIdx);
			}
		}

		TArray<int32> UsedVertexIndices = UsedVertexIndicesSet.Array();
		UsedVertexIndices.Sort(); // Consistent ordering

		// Create section vertices and remapping
		TArray<FSoftSkinVertex> SectionVertices;
		TMap<int32, int32> VertexRemap;

		for (int32 OldIndex : UsedVertexIndices)
		{
			VertexRemap.Add(OldIndex, SectionVertices.Num());
			SectionVertices.Add(AllVertices[OldIndex]);
		}

		// Add indices to the buffer with remapping
		TArray<int32> SectionIndices;
		for (int32 i = 0; i < TriangleIndices.Num(); i += 3)
		{
			int32 I0 = TriangleIndices[i + 0];
			int32 I1 = TriangleIndices[i + 1];
			int32 I2 = TriangleIndices[i + 2];

			// Validate indices
			if (!VertexRemap.Contains(I0) || !VertexRemap.Contains(I1) || !VertexRemap.Contains(I2))
			{
				UE_LOG(LogTemp, Warning, TEXT(" Invalid triangle indices in material %d"), MaterialIndex);
				continue;
			}

			SectionIndices.Add(VertexRemap[I0]);
			SectionIndices.Add(VertexRemap[I1]);
			SectionIndices.Add(VertexRemap[I2]);
		}

		// Add indices to global buffer
		for (int32 Idx : SectionIndices)
		{
			LODModel->IndexBuffer.Add(Idx + LODModel->NumVertices);
		}

		// Create the section
		FSkelMeshSection NewSection;
		NewSection.MaterialIndex = MaterialIndex;
		NewSection.BaseVertexIndex = LODModel->NumVertices;
		NewSection.NumVertices = SectionVertices.Num();
		NewSection.BaseIndex = BaseIndex;
		NewSection.NumTriangles = SectionIndices.Num() / 3;
		NewSection.bUse16BitBoneIndex = GlobalBoneMap.Num() <= 65536;
		NewSection.SoftVertices = SectionVertices;
		NewSection.BoneMap = GlobalBoneMap;

		LODModel->Sections.Add(NewSection);
		LODModel->NumVertices += SectionVertices.Num();
		BaseIndex += SectionIndices.Num();

		//UE_LOG(LogTemp, Log, TEXT(" Created section for material %d: %d vertices, %d triangles"),
			//MaterialIndex, SectionVertices.Num(), NewSection.NumTriangles);
	}

	// Set mesh bounds
	FBox BoundingBox(ForceInit);
	for (const FVector3f& V : Vertices)
	{
		BoundingBox += FVector(V);
	}
	OutLimbMesh->SetImportedBounds(FBoxSphereBounds(BoundingBox));

	// Add LOD info if needed
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

	// Copy materials from source mesh
	const TArray<FSkeletalMaterial>& SourceMaterials = SourceMesh->GetMaterials();
	if (SourceMaterials.Num() > 0)
	{
		OutLimbMesh->SetMaterials(SourceMaterials);
	}
	else
	{
		// Add a default material if source has none
		FSkeletalMaterial DefaultMaterial;
		DefaultMaterial.MaterialInterface = UMaterial::GetDefaultMaterial(MD_Surface);
		TArray<FSkeletalMaterial> DefaultMaterials;
		DefaultMaterials.Add(DefaultMaterial);
		OutLimbMesh->SetMaterials(DefaultMaterials);
	}

	// Finalize mesh settings
	LODModel->NumTexCoords = 1;
	OutLimbMesh->SetHasVertexColors(Colors.Num() > 0);
	OutLimbMesh->SetEnablePerPolyCollision(false);

	// Calculate inverse reference matrices and finalize
	OutLimbMesh->CalculateInvRefMatrices();
	OutLimbMesh->PostEditChange();

	UE_LOG(LogTemp, Log, TEXT("✅ Successfully created limb mesh with %d vertices, %d triangles, %d sections"),
		LODModel->NumVertices, LODModel->IndexBuffer.Num() / 3, LODModel->Sections.Num());

#else
	UE_LOG(LogTemp, Error, TEXT("❌ WITH_EDITORONLY_DATA required for mesh creation"));
#endif
}

void ULimbExtractorTool::ClearCachedDataForRebuild()
{
	RawVertexData.Empty();
	CachedMeshContext.Clear();
	CapComponents.Empty();
	OnEditorUtilityClosed(); // clears actors too
}

// the main functions for extracting vertices, process data and building the skeletal mesh, fade rings, and gore caps - also spawns the preview actor
void ULimbExtractorTool::CreateSkeletalMesh(USkeletalMesh* SkeletalMesh,FName BoneName, bool CreateBoneChain)
{
	// This has to be cleared each time so it holds fresh data
	ClearCachedDataForRebuild();

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

	auto Reporter = UModalTaskHelper::CreateProgressReporter(4/*Number of major phases*/, TEXT("Creating Skeletal Mesh section from Skeletal Mesh"));
	FSlateApplication::Get().Tick(); // Forces UI update
	FPlatformProcess::Sleep(0.05f);  // Optional: small delay to allow visual update

#pragma region skelmesh creation
	
		TArray<int32> OutIndicies;
		TMap<int32, int32> BoneReMap;
		TArray<FRawVertexData> VertexData;
		// gets the bone indices associated with the Desired Bone Names children
		GetBoneHierachy(SkeletalMesh, BoneName, ExclBones, OutIndicies);

		Reporter->ReportProgressWithText(0.1f, TEXT("Extracted Bone Hierachy"));

		if (CreateBoneChain)
		{
			TSet<int32> ProcessedIndices;

			// Extract the whole bone chain, or a selection of it if specified - i could make this happen without the loop, but its nice to check each bone
			for (int Index : OutIndicies)
			{
				auto BoneProgress = Reporter->GetCallback();

				FName DesiredBone = SkeletalMesh->GetRefSkeleton().GetBoneName(Index);
				ExtractSkelMeshVertices(SkeletalMesh, DesiredBone, VertexData, MinimumWeight, OutIndicies, ProcessedIndices);

				BoneProgress.ExecuteIfBound(1.0f / OutIndicies.Num());
			}
			Reporter->ReportProgressWithText(1.0f, TEXT("Extracted bone vertices"));

		}
		else
		{
			TSet<int32> ProcessedIndices;

			ExtractSkelMeshVertices(SkeletalMesh, BoneName, VertexData, MinimumWeight, OutIndicies, ProcessedIndices);
			
			Reporter->ReportProgressWithText(1.0f, TEXT("Extracted bone vertices"));

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
		BuildBuffersFromRawVertexData(SkeletalMesh, VertexData, FinalVertices, FinalNormals, FinalUVs, FinalColors, FinalTangents, FinalVertexIndices, Skin, IntVecTriangles,MaterialIndices, 0);
		
		Reporter->ReportProgressWithText(1.0f, TEXT("Built VertexData buffers"));

		// Super Important!! check for valid data or it'll crash
		if (VertexData.Num() == 0)
		{
			// show an error message the user can get feedback from and acknowledge to cancel the extraction 
			UModalTaskHelper::ShowBlockingConfirmation(TEXT("Extracted Vertices array check"), TEXT("\n Returning due to insufficient vertex data"));

			UE_LOG(LogTemp, Error, TEXT("Returning from no vertex data"));
			return;
		}
	

		///  Double check this and why the array out of bounds error occurred... would be better than having all these buffers instead of utilizing the actual vertexData array
		CreateLimbMeshFromRawVertexData(SkeletalMesh, OutLimbSkelMesh, FinalVertices, FinalUVs, FinalNormals, FinalColors, FinalTangents, IntVecTriangles, VertexData, BoneReMap, SkinWeightBoneIndicesUsed, MaterialIndices);
		
		Reporter->ReportProgressWithText(1.0f, TEXT("Created Limb Mesh"));

		// obviously spawns out actor and attaches the skeletal mesh to it so we have something to inspect
		SpawnSkelMeshActor();
		//After each build, clear the array for a fresh start on the next mesh build ** pain in the ass tho to remember re-adding the child-depth bone 
		ExcludedBoneNames.Empty();
		// We populate this, so we can use it in the MeshMergeUtility to create a merged SkelMesh/Caps mesh and show it in the persona tab to check it
		RawVertexData = VertexData;

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
#pragma endregion



		// Do a check before trying to generate a mesh to make sure we have the required data.. ie arrays with values
		if (FinalVertices.Num() == 0 || FinalNormals.Num() == 0 || FinalUVs.Num() == 0 || FinalColors.Num() == 0 || MeshTangents.Num() == 0 || FinalVertexIndices.Num() == 0 || /*OldToNewVertexMap.Num() == 0 ||*/ FinalTriangles.Num() == 0)
		{
			// show an error popup with feedback the user can acknowledge to cancel building
			UModalTaskHelper::ShowBlockingConfirmation(TEXT("Final buffers array check"), TEXT("a buffer was empty - returning due to insufficient vertex data"));

			UE_LOG(LogTemp, Warning, TEXT("FinV = %d, FinN = %d, FinUV = %d, FC = %d, MT = %d, FVI = %d, OTNVM = %d, FT = %d"), FinalVertices.Num(), FinalNormals.Num(), FinalUVs.Num(), FinalColors.Num(), MeshTangents.Num(), FinalVertexIndices.Num(), OldToNewVertexMap.Num(), FinalTriangles.Num());
			return;
		}


#pragma region fill our CachedMeshContext for use in the edgeloop info building

		
		// fill our Cached Mesh Context so we can use it in the capping section
		CachedMeshContext.FinalMaterialIndices = MaterialIndices;
		CachedMeshContext.FinalNormals = FinalNormals;
		CachedMeshContext.FinalTangents = FinalTangents;
		CachedMeshContext.FinalTriangles = FinalTriangles;
		CachedMeshContext.FinalUVs = FinalUVs;
		CachedMeshContext.FinalVertices = FinalVertices;
		CachedMeshContext.IntVecTriangles = IntVecTriangles;

		Reporter->ReportProgressWithText(1.0f, TEXT("Filled the CachedMeshContext buffers"));
		FPlatformProcess::Sleep(0.05); // sleep for a tick so the user can see the last message

		Reporter.Reset();  // Destroy the reporter so anotherr can be created in the next function nn
#pragma endregion

		// takes the cached info and makes edgeloop data for the ring and cap creation
		BuildEdgeLoopInfoCache(SkeletalMesh);
	
#pragma endregion
}

void ULimbExtractorTool::CreateStaticMesh(USkeletalMesh* SkeletalMesh, FName BoneName, bool CreateBoneChain)
{

	ClearCachedDataForRebuild();

#pragma region extract bone data and create data from it

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
	TArray<int32> OutIndicies;
	TMap<int32, int32> BoneReMap;
	
	
	auto Reporter = UModalTaskHelper::CreateProgressReporter(5/*Number of major phases*/, TEXT("Creating Static Mesh from Skeletal Mesh"));
	FSlateApplication::Get().Tick(); // Forces UI update
	FPlatformProcess::Sleep(0.05f);  // Optional: small delay to allow visual update

	// gets the bone indices associated with the Desired Bone Names children
	GetBoneHierachy(SkeletalMesh, BoneName, ExclBones, OutIndicies);

	Reporter->ReportProgressWithText(1.0f, TEXT("Extracted Bone Hierachy"));
	
	if (CreateBoneChain) // get the whole bone chain worth of verts
	{
		// Extract the whole bone chain, or a selection of it if specified - i could make this happen without the loop, but its nice to check each bone
		for (int Index : OutIndicies)
		{
			auto BoneProgress = Reporter->GetCallback();
			// as we cycle through the outindices, find the bone name to give to the extraction function
			FName DesiredBone = SkeletalMesh->GetRefSkeleton().GetBoneName(Index);
			// Extract the bone vertices from the mesh associated with the bone its skinned to
			ExtractBoneVertices(SkeletalMesh, DesiredBone, FinalVertices, FinalVertexIndices, MinimumWeight /*= preset to 255 until further notice */, SelectedVertices);

			BoneProgress.ExecuteIfBound(1.0f / OutIndicies.Num());
			
		}
		Reporter->ReportProgressWithText(1.0f, TEXT("Extracted bone vertices"));

	}
	else  //  get only the chosen bones verts
	{
		ExtractBoneVertices(SkeletalMesh, BoneName, FinalVertices, FinalVertexIndices, MinimumWeight /*= 128*/, SelectedVertices);
	
		Reporter->ReportProgressWithText(1.0f, TEXT("Extracted bone vertices"));
	}
	
	// Super important !! If we have no vertex data, the system will crashh and hang due to no SkeletalRenderData
	if (FinalVertices.Num() == 0)
	{
		// show an error message the user can get feedback from and acknowledge to cancel the extraction 
		UModalTaskHelper::ShowBlockingConfirmation(TEXT("Extracted Vertices array check"), TEXT("\n Returning due to insufficient vertex data"));

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

	Reporter->ReportProgressWithText(1.0f, TEXT("Built static mesh buffers"));
	
#pragma endregion

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
	
	Reporter->ReportProgressWithText(1.0f, TEXT("Mapped materials to triangles"));
	
#pragma endregion		

	
	// Do a check before trying to generate a mesh to make sure we have the required data.. ie arrays with values
	if (FinalVertices.Num() == 0 || FinalNormals.Num() == 0 || FinalUVs.Num() == 0 || FinalColors.Num() == 0 || MeshTangents.Num() == 0 || FinalVertexIndices.Num() == 0 || OldToNewVertexMap.Num() == 0 || FinalTriangles.Num() == 0)
	{
		// show an error popup with feedback the user can acknowledge to cancel building
		UModalTaskHelper::ShowBlockingConfirmation(TEXT("Final buffers array check"), TEXT("a buffer was empty - returning due to insufficient vertex data"));

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
	
	Reporter->ReportProgressWithText(1.0f, TEXT("Created procedural mesh sections"));
	
	// fill our Cached Mesh Context so we can use it in the capping section
	CachedMeshContext.FinalMaterialIndices = MaterialIndices;
	CachedMeshContext.FinalNormals = FinalNormals;
	CachedMeshContext.FinalTangents = FinalTangents;
	CachedMeshContext.FinalTriangles = FinalTriangles;
	CachedMeshContext.FinalUVs = FinalUVs;
	CachedMeshContext.FinalVertices = FinalVertices;
	CachedMeshContext.MeshMaterials = SkeletalMaterials;
	
	CachedMeshContext.IntVecTriangles = IntVecTriangles;

	Reporter->ReportProgressWithText(1.0f, TEXT("Filled the CachedMeshContext buffers"));
	FPlatformProcess::Sleep(0.05); // sleep for a tick so the user can see the last message

#pragma endregion
	Reporter.Reset();  // Destroy the reporter so another can be created in the next function 
	// takes the cached info and makes edgeloop data for the ring and cap creation
	BuildEdgeLoopInfoCache(SkeletalMesh);
	
}

// this takes our extracted data, cleans it, finds valid edgeloops and broadcasts the data to the menu
// Enhanced BuildEdgeLoopInfoCache function (replace your existing one)
void ULimbExtractorTool::BuildEdgeLoopInfoCache(USkeletalMesh* Mesh)
{

	EdgeLoopInfos.Reset();
	ClusteredEdgeLoopInfos.Reset();
	DetectedClusters.Reset();

	UE_LOG(LogTemp, Warning, TEXT("Start of BuildEdgeLoopInfoCache: IntVecTriangles.Num() = %d"),
		CachedMeshContext.IntVecTriangles.Num());

	// Progress reporter with additional steps for clustering
	int32 TotalSteps = bEnableLoopClustering ? 8 : 6;
	auto Reporter = UModalTaskHelper::CreateProgressReporter(TotalSteps, TEXT("Building EdgeLoop Info Cache with Clustering"));

	// Your existing setup code
	TArray<FVector3f> FinalVertices = CachedMeshContext.FinalVertices;
	TArray<FIntVector> IntVecTriangles = CachedMeshContext.IntVecTriangles;
	TArray<FVector2d> FinalUVs = CachedMeshContext.FinalUVs;
	TArray<FVector> FinalNormals = CachedMeshContext.FinalNormals;
	TArray<int32> FinalTriangles = CachedMeshContext.FinalTriangles;
	TArray<int32> MaterialIndices = CachedMeshContext.FinalMaterialIndices;
	int32 SectionIndex = 0;
	USkeletalMesh* SkeletalMesh = Mesh;

	FPlatformProcess::Sleep(0.1f);
	Reporter->ReportProgressWithText(1.0f, TEXT("UMeshCleanUtils cleaning the mesh for capping"));

	// Your existing mesh cleaning
	UMeshCleanUtils::UtilCleanMeshForCapping(FinalVertices, IntVecTriangles, FinalUVs, FinalNormals, 1.01f, 10, false);

	// Your existing triangle conversion
	TArray<int32> TempTriangles;
	for (const FIntVector V : IntVecTriangles)
	{
		TempTriangles.Add(V.X);
		TempTriangles.Add(V.Y);
		TempTriangles.Add(V.Z);
	}

	Reporter->ReportProgressWithText(1.0f, TEXT("Finding open edges"));
	TSet<TPair<int32, int32>> OpenEdges = FindOpenEdgesFromTriangles(TempTriangles);

	Reporter->ReportProgressWithText(1.0f, TEXT("Sorting open edges into edgeloops"));
	TArray<TArray<int32>> EdgeLoops = SortOpenEdgesIntoLoops(OpenEdges, true);

	Reporter->ReportProgressWithText(1.0f, TEXT("Checking for valid loops"));

	// Your existing loop processing with slight modifications
	TArray<TArray<int32>> CleanedLoops;
	TArray<FEdgeLoopInfo> EdgeLoopInfoArray;
	int32 LoopNum = 0;

	for (const TArray<int32>& Loop : EdgeLoops)
	{
		auto BoneProgress = Reporter->GetCallback();

		if (Loop.Num() > 4)
		{
			CleanedLoops.Add(Loop);

			FEdgeLoopInfo Info;
			Info.VertexIndices = Loop;

			for (int32 Index : Loop)
			{
				if (FinalVertices.IsValidIndex(Index))
				{
					Info.Positions.Add(FinalVertices[Index]);
					Info.UVs.Add(FinalUVs[Index]);
				}
			}

			Info.VotedMaterialIndex = InferLoopMaterialFromPositions(Info.Positions, FinalTriangles, FinalVertices, MaterialIndices, 0.001f);
			Info.LoopNumber = LoopNum;

			UE_LOG(LogTemp, Warning, TEXT("Raw Loop %d with %d vertices"), LoopNum, Loop.Num());

			EdgeLoopInfoArray.Add(Info);
			LoopNum++;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Raw Detected Loop with %d vertices - skipping"), Loop.Num());
		}

		BoneProgress.ExecuteIfBound(1.0f / EdgeLoops.Num());
	}

	// Save the EdgeloopInfo so it can be accessed later
	EdgeLoopInfos = EdgeLoopInfoArray;
	
	// NEW: Apply clustering if enabled  ** needs work - loop sections joining needs more controls  
	/*if (bEnableLoopClustering)
	{
		Reporter->ReportProgressWithText(1.0f, TEXT("Clustering edge loops"));
		ProcessEdgeLoopsWithClustering(EdgeLoopInfoArray);

		Reporter->ReportProgressWithText(1.0f, TEXT("Merging clustered loops"));
		// EdgeLoopInfoArray is now modified with clustered results

		// Save the EdgeloopInfo (now potentially clustered)
		//ClusteredEdgeLoopInfos = EdgeLoopInfoArray; // Keep a separate copy for reference4
		//EdgeLoopInfos = EdgeLoopInfoArray;  this sets it back since edgeloopinfoarray is only set to the initial edgeloopinfos before clustering
		UE_LOG(LogTemp, Warning, TEXT("Clustering section completed. Final EdgeLoopInfo loops: %d, Final Edgeloopinfoarray loops: %d"), EdgeLoopInfos.Num(), EdgeLoopInfoArray.Num());


	}*/


	Reporter->ReportProgressWithText(1.0f, TEXT("Populating menu data"));
	FPlatformProcess::Sleep(0.05);
	Reporter.Reset();

	// wrapper and broadcast to menu
	Wrapper.EdgeLoops = EdgeLoopInfos;
	Prompt.Broadcast(Wrapper);

	UE_LOG(LogTemp, Warning, TEXT("BuildEdgeLoopInfoCache completed. Final loops: %d"), EdgeLoopInfos.Num());
}

// our main cappinng function that creates the fade ring and cap mesh.   Populated by BuildEdgeLoopInfoCache ** called by blueprint in the menu ** 
void ULimbExtractorTool::CreateFadeRingAndCap(FEdgeLoopInfo LoopInfo, FEdgeLoopInfo& UpdatedInfo, FCapSkinningInfo Info)
{
	// set our UV variables to use in the dome generator
	BP_UVScale = LoopInfo.UV_Scale;
	BP_UVRotationDegrees = LoopInfo.UV_Rotation;
	BP_UVOffset = FVector2D(LoopInfo.UV_Offset_X, LoopInfo.UV_Offset_Y);
	BP_RCFRUVScale = LoopInfo.UV_RCFRScale;
	BP_RCFRUVRotationDegrees = LoopInfo.UV_RCFRRotation;
	BP_RCFRUVOffset = FVector2D(LoopInfo.UV_RCFROffset_X, LoopInfo.UV_RCFROffset_Y);
	
	// Destroy old mesh if it exists
	if (CapComponents.Contains(LoopInfo.LoopNumber))
	{
		UProceduralMeshComponent* OldMesh = CapComponents[LoopInfo.LoopNumber];
		if (OldMesh && OldMesh->IsValidLowLevel())
		{
			OldMesh->DestroyComponent();
		}
		CapComponents.Remove(LoopInfo.LoopNumber);

		if (CapSkinningInfos.Contains(LoopInfo.LoopNumber))
		{
			CapSkinningInfos.Remove(LoopInfo.LoopNumber);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("For some reason we are out of sync with the capComponents - CSI %d, CC %d"), CapSkinningInfos.Num(), CapComponents.Num());
		}
	}	

#pragma region create edgeloop info for generators and mesh generation
	
	// Create buffers
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
			LoopPositions.Add(CachedMeshContext.FinalVertices[Index]); // this is now out of order because we dont write back to cachedmeshcontext after welding.  ** need a fix here, or just use loopinfo.positions **
			LoopUVs.Add(CachedMeshContext.FinalUVs[Index]); // aligns positions and UVs	
		}
	}

	int32 LoopMaterialIndex = 0; // Default fallback

	// Check if user has specified a custom fade ring material
	bool bUseCustomFadeRingMaterial = false;
	if (LoopInfo.FadeRingMaterial.Get())
	{
		bUseCustomFadeRingMaterial = true;
	}
	else
	{
		LoopMaterialIndex = InferLoopMaterialFromPositions(LoopPositions, CachedMeshContext.FinalTriangles, CachedMeshContext.FinalVertices, CachedMeshContext.FinalMaterialIndices, 0.001f);
	}


	// generate a fade ring, or use the input edgeloop
	if (LoopInfo.bAddFadeRing)
	{
		TArray<int32> FadeRingLoop;
		TArray<int32> FadeRingTriangles;
		TArray<FLinearColor> FadeRingColors;
		TArray<FProcMeshTangent> FadeRingTangents;
		TArray<FVector2d> FadeRingUVs;
		TArray<FVector> FadeRingNormals;

		//bool twist = IsLoopTwistRisk(LoopInfo.Positions, LoopInfo.PlanitaryThreshold, LoopInfo.MaxNormalDeviation, LoopInfo.SegmentSlopeTolerance, LoopInfo.SegmentWindow);
		// check to see if the loop will give malformed results and choose the correct fade ring generator accordingly (should condense this but.. eh.. )
		bool bforcedbool = true;
		if (bforcedbool)//(LoopInfo.TwistFixOverride)// || twist)  // currrently set to true until the standard faderingandcap function works better
		{
			//UE_LOG(LogTemp, Warning, TEXT("Forced RobustFadeRing path- /*Edge loop flagged as twist risk, Running safe projection method*/."));
			GenerateRobustFadeRingCapFromLoopInfo(LoopInfo, CapVertices, FadeTriangles, FadeRingUVs, FadeRingNormals, FadeRingTangents, FadeRingColors, FadeRingLoop, FadeRingTriangles);

			// Copy fade ring data to main buffers
			CapUVs.Append(FadeRingUVs);
			CapNormals.Append(FadeRingNormals);
			CapTangents.Append(FadeRingTangents);
			CapColors.Append(FadeRingColors);
		}
		else  // not used atm until further notice as the output geometry needs more normalizing calc via input bone
		{
			// For the old function, we need to generate placeholder tangents and colors
			TArray<FVector2D> OldFadeUVs;
			TArray<FVector> OldFadeNormals;
			GenerateFadeRingCapFromLoopInfo(LoopInfo, CapVertices, FadeTriangles, OldFadeUVs, OldFadeNormals, FadeRingLoop, FadeRingTriangles);

			// Convert and append old function outputs
			CapUVs.Append(OldFadeUVs);
			CapNormals.Append(OldFadeNormals);

			// Generate placeholder tangents and colors for old function
			FVector3f P0 = LoopInfo.Positions[0];
			FVector3f P1 = LoopInfo.Positions[LoopInfo.Positions.Num() / 2];
			FVector3f LimbAxis = (P1 - P0).GetSafeNormal();
			FVector3f SliceNormal = CapNormals.Num() > 0 ? FVector3f(CapNormals.Last()) : FVector3f::UpVector;
			FVector3f TangentDir = FVector3f::CrossProduct(SliceNormal, LimbAxis).GetSafeNormal();

			for (int32 i = CapTangents.Num(); i < CapVertices.Num(); ++i)
			{
				CapTangents.Add(FProcMeshTangent(FVector(TangentDir), false));
				CapColors.Add(FLinearColor::White);
			}
		}

		// assign a material indice to each triangle
		if (bUseCustomFadeRingMaterial)
		{
			// Use a special index to identify custom material triangles
			MaterialToTriangles.FindOrAdd(-1).Append(FadeRingTriangles);
		}
		else
		{
			// Use inferred material index
			MaterialToTriangles.FindOrAdd(LoopMaterialIndex).Append(FadeRingTriangles);
		}
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
			LoopInfo.FadeRingPositions = RingVerts;

		}


	}
	else
	{
		// Since we dont have a fadeloop, we just populate the DomeEdgeLoop with the original found edgeloop
		DomeEdgeLoops.Add(LoopInfo.Positions);//LoopPositions);
		LoopInfo.FadeRingPositions = LoopInfo.Positions;
		CapVertices = LoopPositions;
		CapUVs = LoopUVs;

		// Generate placeholder tangents, colors and normals for original loop
		FVector3f P0 = LoopInfo.Positions[0];
		FVector3f P1 = LoopInfo.Positions[LoopInfo.Positions.Num() / 2];
		FVector3f LimbAxis = (P1 - P0).GetSafeNormal();
		FVector3f Tangent = (LoopInfo.Positions[1] - P0).GetSafeNormal();
		FVector3f SliceNormal = FVector3f::CrossProduct(LimbAxis, Tangent).GetSafeNormal();
		FVector3f TangentDir = FVector3f::CrossProduct(SliceNormal, LimbAxis).GetSafeNormal();

		for (int32 i = 0; i < CapVertices.Num(); ++i)
		{
			CapNormals.Add(FVector(SliceNormal));
			CapTangents.Add(FProcMeshTangent(FVector(TangentDir), false));
			CapColors.Add(FLinearColor::White);
		}
	}

#pragma region check array and create cap info	

	// final sanity check so an empty array stack doesnt get passed and crash it
	if (DomeEdgeLoops.Num() == 0 || DomeEdgeLoops[0].Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No edge loops found to cap!"));
		return;
	}

	// Store current buffer sizes before dome/cap generation
	int32 PreCapVertexCount = CapVertices.Num();
	int32 PreCapUVCount = CapUVs.Num();
	int32 PreCapNormalCount = CapNormals.Num();
	int32 PreCapTangentCount = CapTangents.Num();
	int32 PreCapColorCount = CapColors.Num();

	// finally generate the vertices data we want to give to create mesh section
	if (LoopInfo.bAddDome) // add the dome style adjustable cap
	{
		for (const TArray<FVector3f>& Loop1 : DomeEdgeLoops)
		{
			UE_LOG(LogTemp, Warning, TEXT("running dome cap"));

			// Store counts before dome generation
			int32 PreDomeVertexCount = CapVertices.Num();

			GenerateDomedCapFromLoop(Loop1, CapVertices, CapTriangles, CapUVs, CapNormals, LoopInfo.DomeHeight, LoopInfo.NoiseAmount, LoopInfo.RingCount, LoopInfo.bAddNoise);

			// Fill missing tangents and colors for dome vertices
			int32 NewVertexCount = CapVertices.Num() - PreDomeVertexCount;
			FVector3f DomeTangentDir = FVector3f::ForwardVector; // Default dome tangent
			if (Loop1.Num() > 1)
			{
				FVector3f Center(0);
				for (const FVector3f& V : Loop1) Center += V;
				Center /= Loop1.Num();

				FVector3f P0 = Loop1[0];
				FVector3f P1 = Loop1[Loop1.Num() / 2];
				FVector3f LimbAxis = (P1 - P0).GetSafeNormal();
				FVector3f Tangent = Loop1.Num() > 2 ? (Loop1[1] - P0).GetSafeNormal() : FVector3f::RightVector;
				FVector3f SliceNormal = FVector3f::CrossProduct(LimbAxis, Tangent).GetSafeNormal();
				DomeTangentDir = FVector3f::CrossProduct(SliceNormal, LimbAxis).GetSafeNormal();
			}

			for (int32 i = 0; i < NewVertexCount; ++i)
			{
				CapTangents.Add(FProcMeshTangent(FVector(DomeTangentDir), false));
				CapColors.Add(FLinearColor::White);
			}
		}
	}
	else //  or create a fan cap (UVs adjustable only)
	{
		for (const TArray<FVector3f>& Loop1 : DomeEdgeLoops)
		{
			UE_LOG(LogTemp, Warning, TEXT("running fan cap"));

			// Store counts before fan generation
			int32 PreFanVertexCount = CapVertices.Num();

			GenerateCapFanFromLoop(Loop1, CachedMeshContext.FinalVertices, CapVertices, CapTriangles, CapUVs, CapNormals);

			// Fill missing tangents and colors for fan vertices
			int32 NewVertexCount = CapVertices.Num() - PreFanVertexCount;
			FVector3f FanTangentDir = FVector3f::ForwardVector; // Default fan tangent
			if (Loop1.Num() > 1)
			{
				FVector3f P0 = Loop1[0];
				FVector3f P1 = Loop1[Loop1.Num() / 2];
				FVector3f LimbAxis = (P1 - P0).GetSafeNormal();
				FVector3f Tangent = Loop1.Num() > 2 ? (Loop1[1] - P0).GetSafeNormal() : FVector3f::RightVector;
				FVector3f SliceNormal = FVector3f::CrossProduct(LimbAxis, Tangent).GetSafeNormal();
				FanTangentDir = FVector3f::CrossProduct(SliceNormal, LimbAxis).GetSafeNormal();
			}

			for (int32 i = 0; i < NewVertexCount; ++i)
			{
				CapTangents.Add(FProcMeshTangent(FVector(FanTangentDir), false));
				CapColors.Add(FLinearColor::White);
			}
		}

	}

	// if we want to show the edgeloops/faderings for checking
	if (ShowLoopDebug)
	{
		FTransform Transform;
		if (SkelMesh)  // if we have a skelmesh, use its transform
		{
			Transform = SkelMesh->GetComponentTransform();
		}
		else  // otherwise its a static mesh , so use it instead
		{
			Transform = PreviewMesh->GetComponentTransform();
		}
		DrawEdgeLoopsDebug(DomeEdgeLoops, CachedMeshContext.FinalVertices, GetWorld(), Transform, 10, .4);
	}

#pragma endregion

#pragma region fill CapBuffers and generate uv data

	// --- Fill Buffers ---
	TArray<FVector> CapVerts;

	// Ensure all arrays are the same size
	int32 MaxVertexCount = CapVertices.Num();

	// Resize arrays to match if needed
	while (CapNormals.Num() < MaxVertexCount)
	{
		CapNormals.Add(FVector(1, 0, 0)); // Placeholder normal
	}
	while (CapUVs.Num() < MaxVertexCount)
	{
		CapUVs.Add(FVector2D(0.5f, 0.5f)); // Placeholder UV
	}
	while (CapTangents.Num() < MaxVertexCount)
	{
		CapTangents.Add(FProcMeshTangent(1, 0, 0)); // Placeholder tangent
	}
	while (CapColors.Num() < MaxVertexCount)
	{
		CapColors.Add(FLinearColor::Red); // Placeholder color
	}

	// Convert FVector3f to FVector
	for (int32 i = 0; i < CapVertices.Num(); ++i)
	{
		CapVerts.Add(FVector(CapVertices[i]));
	}

#pragma endregion

	// WE MUST HAVE FILLED ARRAYS FROM PREVIOUS CODE FOR CAPVERTS, CAPTRIANGLES, and the others
# pragma region create capmesh
		// --- CREATE THE MESH ---
	FString Name = FString::Printf(TEXT("CapMesh_%d"), LoopInfo.LoopNumber);
	UProceduralMeshComponent* CapMesh = NewObject<UProceduralMeshComponent>(OwnerActor, *Name);

	CapMesh->RegisterComponentWithWorld(GetWorld());

	if (SkelMesh->IsValidLowLevel())
	{
		CapMesh->AttachToComponent(SkelMesh, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else if (PreviewMesh)
	{
		CapMesh->AttachToComponent(PreviewMesh, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No valid mesh to attach CapMesh to!"));
		return;
	}

	CapMesh->SetWorldLocation(SkelMesh->IsValidLowLevel() ? SkelMesh->GetComponentLocation() : PreviewMesh->GetComponentLocation());
	CapMesh->bUseComplexAsSimpleCollision = true;
	CapMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapMesh->SetVisibility(true);

	int32 SectionIndex = 0;

	//  Add all Fade Ring triangles (with inherited materials)
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

		// Handle custom fade ring material assignment
		if (MaterialIndex == -1 && LoopInfo.FadeRingMaterial.Get())
		{
			// Assign custom fade ring material
			CapMesh->SetMaterial(SectionIndex, LoopInfo.FadeRingMaterial.Get());
			UE_LOG(LogTemp, Warning, TEXT("Assigned custom fade ring material: %s"), *LoopInfo.FadeRingMaterial.Get()->GetName());
		}
		else if (InMesh->GetMaterials().IsValidIndex(MaterialIndex))
		{
			// Use inherited material from base mesh
			CapMesh->SetMaterial(SectionIndex, InMesh->GetMaterials()[MaterialIndex].MaterialInterface);

			UMaterialInterface* Mat = InMesh->GetMaterials()[MaterialIndex].MaterialInterface;
			if (Mat)
			{
				UE_LOG(LogTemp, Warning, TEXT("Assigned inherited material to fade rings: %s"), *Mat->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid MaterialIndex %d, using default material"), MaterialIndex);

			// Fallback to default material
			UMaterial* DefaultMaterial = nullptr;
			if (GEngine)
			{
				DefaultMaterial = GEngine->DebugEditorMaterial;
			}
			CapMesh->SetMaterial(SectionIndex, DefaultMaterial);
		}

		++SectionIndex;
	}

	// Add capping geometry (gore/cap section)
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

		if (LoopInfo.CapMaterial.Get())
		{
			CapMesh->SetMaterial(SectionIndex, LoopInfo.CapMaterial.Get());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve the LoopInfo Material, so trying for the world default."));

			UMaterial* DefaultMaterial = nullptr;

			if (GEngine)
			{
				DefaultMaterial = GEngine->DebugEditorMaterial;
			}

			CapMesh->SetMaterial(SectionIndex, DefaultMaterial);
		}
	}
	

	// Store new mesh in map and sync up the cap infos.  These must always be in sync with the LoopInfo.LoopNumber
	CapComponents.Add(LoopInfo.LoopNumber, CapMesh);
	CapSkinningInfos.Add(LoopInfo.LoopNumber, Info);

	// in case we want to use the data for some reason in BP..
	UpdatedInfo = LoopInfo;
	// Empty the Wrapper struct for the next cap
	Wrapper.EdgeLoops.Empty();

	// create a reverse cap preview in case the user wants one
	CreateReverseCap(LoopInfo);

#pragma endregion
	// all done
}



void ULimbExtractorTool::CreateReverseCap(FEdgeLoopInfo LoopInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("Starting reverse cap generation using stored FadeRingPositions"));

	if (LoopInfo.FadeRingPositions.Num() < 3)
	{
		UE_LOG(LogTemp, Error, TEXT("FadeRingPositions not populated or too small"));
		return;
	}

	// Step 1: Compute center
	FVector3f Center(0);
	for (const FVector3f& V : LoopInfo.FadeRingPositions)
		Center += V;
	Center /= LoopInfo.FadeRingPositions.Num();

	// Step 2: Expand stored ring outward
	TArray<FVector3f> ExpandedRing;
	for (const FVector3f& V : LoopInfo.FadeRingPositions)
	{
		FVector3f ToCenter = (Center - V).GetSafeNormal();
		ExpandedRing.Add(V - ToCenter * LoopInfo.LimbEndCollapseAmount);
	}

	// Step 3: Generate fade ring from expanded ring
	FEdgeLoopInfo ReverseLoopInfo;
	ReverseLoopInfo.Positions = ExpandedRing;//ExpandedRing;
	ReverseLoopInfo.VertexIndices.SetNum(ExpandedRing.Num());
	for (int32 i = 0; i < ExpandedRing.Num(); ++i)
		ReverseLoopInfo.VertexIndices[i] = i;

	ReverseLoopInfo.RevCapExpansionAmount = LoopInfo.RevCapExpansionAmount; // to 
	ReverseLoopInfo.ReverseCapFadeAmount = LoopInfo.ReverseCapFadeAmount;
	ReverseLoopInfo.NoiseAmount = LoopInfo.NoiseAmount;
	ReverseLoopInfo.bAddNoise = LoopInfo.bAddNoise;
	ReverseLoopInfo.RingCount = LoopInfo.RingCount;

	TArray<FVector3f> FadeRingVerts;
	TArray<int32> FadeRingTriangles;
	TArray<FVector2D> FadeRingUVs;
	TArray<FVector> FadeRingNormals;
	TArray<int32> FadeRingLoop;
	TArray<FProcMeshTangent> FadeRingTangents;
	TArray<FLinearColor> FadeRingVertexColors;

	UE_LOG(LogTemp, Warning, TEXT("=== INPUT DEBUG ==="));
	for (int32 i = 0; i < LoopInfo.Positions.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoopInfo.Positions[%d]: %s"), i, *LoopInfo.Positions[i].ToString());
	}

	GenerateReverseFadeRingFromLoopInfo(
		ReverseLoopInfo,
		FadeRingVerts,
		FadeRingTriangles,
		FadeRingUVs,
		FadeRingNormals,
		FadeRingTangents,
		FadeRingVertexColors,
		FadeRingLoop,
		FadeRingTriangles
	);

	// Step 4: Generate cap geometry from expanded ring
	TArray<FVector3f> CapVertices;
	TArray<int32> CapTriangles;
	TArray<FVector2D> CapUVs;
	TArray<FVector> CapNormals;

	if (LoopInfo.bAddDome)
	{
		UE_LOG(LogTemp, Warning, TEXT("Generating domed reverse cap"));
		GenerateDomedCapFromLoop(ExpandedRing, CapVertices, CapTriangles, CapUVs, CapNormals,
			LoopInfo.DomeHeight, LoopInfo.NoiseAmount, LoopInfo.RingCount, LoopInfo.bAddNoise);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Generating fan-style reverse cap"));
		GenerateCapFanFromLoop(ExpandedRing, CachedMeshContext.FinalVertices, CapVertices, CapTriangles, CapUVs, CapNormals);
	}

	// Step 5: Flip triangle winding for cap
	TArray<int32> FlippedCapTriangles;
	for (int32 i = 0; i < CapTriangles.Num(); i += 3)
	{
		FlippedCapTriangles.Add(CapTriangles[i]);
		FlippedCapTriangles.Add(CapTriangles[i + 2]); // Swap
		FlippedCapTriangles.Add(CapTriangles[i + 1]);
	}

	// Step 6: Combine fade ring + cap geometry
	TArray<FVector3f> AllVerts = FadeRingVerts;
	AllVerts.Append(CapVertices);

	TArray<int32> AllTriangles = FadeRingTriangles;
	for (int32 i = 0; i < FlippedCapTriangles.Num(); ++i)
		AllTriangles.Add(FlippedCapTriangles[i] + FadeRingVerts.Num()); // Offset

	TArray<FVector2D> AllUVs = FadeRingUVs;
	AllUVs.Append(CapUVs);

	TArray<FVector> AllNormals = FadeRingNormals;
	AllNormals.Append(CapNormals);

	TArray<FLinearColor> AllColors = FadeRingVertexColors;
	TArray<FProcMeshTangent> AllTangents = FadeRingTangents;
	for (int32 i = 0; i < AllVerts.Num(); ++i)
	{
		AllColors.Add(FLinearColor::Red);
		AllTangents.Add(FProcMeshTangent(1, 0, 0));
	}

	// Convert to FVector
	TArray<FVector> FinalVerts;
	for (const FVector3f& V : AllVerts)
		FinalVerts.Add(FVector(V));

	// Step 7: Spawn actor and create mesh
	SpawnRevCapActor(LoopInfo.LoopNumber);
	AActor* RevCapActor = RevCapActors.Contains(LoopInfo.LoopNumber) ? RevCapActors[LoopInfo.LoopNumber] : nullptr;
	if (!RevCapActor)
	{
		UE_LOG(LogTemp, Error, TEXT("No RevCap actor found for index %d"), LoopInfo.LoopNumber);
		return;
	}

	UProceduralMeshComponent* ReverseCap = NewObject<UProceduralMeshComponent>(RevCapActor, *FString::Printf(TEXT("ReverseCap_%d"), LoopInfo.LoopNumber));
	RevCapActor->SetRootComponent(ReverseCap);
	ReverseCap->RegisterComponentWithWorld(GetWorld());
	RevCapActor->AddInstanceComponent(ReverseCap);

	ReverseCap->CreateMeshSection_LinearColor(
		0,
		FinalVerts,
		AllTriangles,
		AllNormals,
		AllUVs,
		AllColors,
		AllTangents,
		true
	);

	// Step 8: Apply material
	if (LoopInfo.CapMaterial.Get())
	{
		ReverseCap->SetMaterial(0, LoopInfo.CapMaterial.Get());
	}
	else
	{
		UMaterial* DefaultMaterial = GEngine ? GEngine->DebugEditorMaterial : nullptr;
		ReverseCap->SetMaterial(0, DefaultMaterial);
	}
}

// The buffer filler for the skelmesh 
void ULimbExtractorTool::BuildBuffersFromRawVertexData(USkeletalMesh* SourceMesh,const TArray<FRawVertexData>& VertexData,TArray<FVector3f>& OutVertices,TArray<FVector>& OutNormals,TArray<FVector2D>& OutUVs,TArray<FColor>& OutColors,TArray<FVector>& OutTangents,TArray<int32>& OutOriginalVertexIndices,TArray<FSkinWeightInfo>& OutSkinWeights,TArray<FIntVector>& OutTriangles, TArray<int32>& OutMaterialIndicesPerTriangle,int32 LODIndex)
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

const FSkeletalMeshLODRenderData& LODData = RenderData->LODRenderData[LODIndex];
const FStaticMeshVertexBuffers& Buffers = LODData.StaticVertexBuffers;
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
	// Use the extracted vertex data directly
	OutVertices.Add(V.Position);
	OutOriginalVertexIndices.Add(V.OriginalVertexIndex);
	OldToNewIndex.Add(V.OriginalVertexIndex, i);

	// Get other vertex attributes from the source mesh buffers using ORIGINAL vertex index
	OutNormals.Add(FVector(Buffers.StaticMeshVertexBuffer.VertexTangentZ(V.OriginalVertexIndex)));
	OutTangents.Add(FVector(Buffers.StaticMeshVertexBuffer.VertexTangentX(V.OriginalVertexIndex)));
	OutColors.Add(FColor::White);
	OutUVs.Add(FVector2D(Buffers.StaticMeshVertexBuffer.GetVertexUV(V.OriginalVertexIndex, 0)));

	// NEW APPROACH: Convert bone names to NEW skeleton indices
	FSkinWeightInfo WeightInfo;
	FMemory::Memzero(WeightInfo);

	int32 ValidInfluences = 0;
	uint32 TotalWeight = 0;

	// Convert bone names to indices in the NEW skeleton
	for (int32 j = 0; j < V.BoneNames.Num() && ValidInfluences < MAX_TOTAL_INFLUENCES; ++j)
	{
		FName BoneName = V.BoneNames[j];

		const FReferenceSkeleton& skel = OutLimbSkelMesh->GetRefSkeleton();  // OutLimbSkelMesh is USkeletalMesh*
		// Find this bone in the NEW skeleton (the one created by CreateSkeletalLimbData)
		int32 NewBoneIndex = skel.FindBoneIndex(BoneName);

		if (NewBoneIndex != INDEX_NONE)
		{
			WeightInfo.InfluenceBones[ValidInfluences] = static_cast<uint8>(NewBoneIndex);
			WeightInfo.InfluenceWeights[ValidInfluences] = static_cast<uint8>(FMath::Clamp(V.BoneWeights[j] * 255.0f, 0.0f, 255.0f));
			TotalWeight += WeightInfo.InfluenceWeights[ValidInfluences];
			ValidInfluences++;
		}
		else
		{
			// This bone doesn't exist in the new skeleton - skip it
			//UE_LOG(LogTemp, Verbose, TEXT("Bone '%s' not found in new skeleton, skipping influence"), *BoneName.ToString());
		}
	}

	// Normalize weights if needed
	if (TotalWeight != 255 && TotalWeight > 0)
	{
		float Scale = 255.0f / TotalWeight;
		uint32 NewTotal = 0;
		for (int32 j = 0; j < ValidInfluences; ++j)
		{
			WeightInfo.InfluenceWeights[j] = FMath::Clamp(FMath::RoundToInt(WeightInfo.InfluenceWeights[j] * Scale), 1, 255);
			NewTotal += WeightInfo.InfluenceWeights[j];
		}
		// Fix rounding errors
		int32 Diff = 255 - NewTotal;
		if (ValidInfluences > 0)
		{
			WeightInfo.InfluenceWeights[0] = FMath::Clamp(WeightInfo.InfluenceWeights[0] + Diff, 1, 255);
		}
	}
	else if (TotalWeight == 0 && ValidInfluences > 0)
	{
		// Fallback: assign full weight to first bone
		WeightInfo.InfluenceWeights[0] = 255;
		//UE_LOG(LogTemp, Warning, TEXT("⚠️ Vertex %d had zero total weight, assigning full weight to first bone"), i);
	}
	else if (ValidInfluences == 0)
	{
		// No valid influences found - assign to root bone (index 0)
		WeightInfo.InfluenceBones[0] = 0;
		WeightInfo.InfluenceWeights[0] = 255;
		ValidInfluences = 1;
		//UE_LOG(LogTemp, Warning, TEXT("⚠️ Vertex %d had no valid influences, assigning to root bone"), i);
	}

	OutSkinWeights.Add(WeightInfo);
}


TMap<int32, int32> IndexBufferOffsetToMaterial;
for (int32 SectionIdx = 0; SectionIdx < LODData.RenderSections.Num(); ++SectionIdx)
{
	const FSkelMeshRenderSection& Section = LODData.RenderSections[SectionIdx];
	const int32 StartIdx = Section.BaseIndex;
	const int32 EndIdx = StartIdx + Section.NumTriangles * 3;

	for (int32 i = StartIdx; i < EndIdx; i += 3)
	{
		IndexBufferOffsetToMaterial.Add(i, Section.MaterialIndex);
	}
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
		//int32 SourceTriIndex = i / 3;
		//int32 MatIndex = /*IndexToSectionMaterial*/IndexBufferOffsetToMaterial.Contains(SourceTriIndex) ? /*IndexToSectionMaterial*/IndexBufferOffsetToMaterial[SourceTriIndex] : 0;
		int32 MatIndex = IndexBufferOffsetToMaterial.Contains(i) ? IndexBufferOffsetToMaterial[i] : 0;
		OutMaterialIndicesPerTriangle.Add(MatIndex);
	}
}

UE_LOG(LogTemp, Warning, TEXT(" Built buffers: %d vertices, %d triangles"), OutVertices.Num(), OutTriangles.Num());
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

	// Create a set for faster lookup of extracted vertices
	TSet<int32> ExtractedVertexSet(ExtractedVertexIndices);

	// Vertex extraction
	for (int32 OldIndex : ExtractedVertexIndices)
	{
		int32 BuffNum = Buffers.PositionVertexBuffer.GetNumVertices();
		if (OldIndex < 0 || OldIndex >= BuffNum) continue;

		int32 NewIndex = OutVertices.Num();
		OutOldToNewVertexMap.Add(OldIndex, NewIndex);
		OutVertices.Add(Buffers.PositionVertexBuffer.VertexPosition(OldIndex));
		OutNormals.Add(FVector(Buffers.StaticMeshVertexBuffer.VertexTangentZ(OldIndex)));
		OutTangents.Add(FVector(Buffers.StaticMeshVertexBuffer.VertexTangentX(OldIndex)));
		OutUVs.Add(FVector2d(Buffers.StaticMeshVertexBuffer.GetVertexUV(OldIndex, 0)));
		OutColors.Add(FColor::White);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Extracted %d vertices from %d total"), OutVertices.Num(), ExtractedVertexIndices.Num());

	// Debug: Track triangles per section
	TMap<int32, int32> SectionTriangleCounts;
	TMap<int32, int32> SectionSkippedCounts;

	// Triangle extraction per section
	for (int32 SectionIdx = 0; SectionIdx < LODData.RenderSections.Num(); ++SectionIdx)
	{
		const FSkelMeshRenderSection& Section = LODData.RenderSections[SectionIdx];
		const int32 BaseIndex = Section.BaseIndex;
		const int32 NumTris = Section.NumTriangles;

		/*UE_LOG(LogTemp, Warning, TEXT("Processing Section %d: Material %d, BaseIndex %d, NumTriangles %d"),
			SectionIdx, Section.MaterialIndex, BaseIndex, NumTris);*/

		int32 ValidTrianglesInSection = 0;
		int32 SkippedTrianglesInSection = 0;

		for (int32 TriIdx = 0; TriIdx < NumTris; ++TriIdx)
		{
			int32 I0 = IndexBuffer->Get(BaseIndex + TriIdx * 3 + 0);
			int32 I1 = IndexBuffer->Get(BaseIndex + TriIdx * 3 + 1);
			int32 I2 = IndexBuffer->Get(BaseIndex + TriIdx * 3 + 2);

			// Check if all vertices are in our extracted set
			bool bHasI0 = ExtractedVertexSet.Contains(I0);
			bool bHasI1 = ExtractedVertexSet.Contains(I1);
			bool bHasI2 = ExtractedVertexSet.Contains(I2);

			if (bHasI0 && bHasI1 && bHasI2)
			{
				// All vertices are in our set - include this triangle
				OutTriangles.Add(FIntVector(
					OutOldToNewVertexMap[I0],
					OutOldToNewVertexMap[I1],
					OutOldToNewVertexMap[I2]
				));
				OutMaterialIndices.Add(Section.MaterialIndex);
				ValidTrianglesInSection++;

				// Debug first few triangles per section
				if (ValidTrianglesInSection <= 3)
				{
					/*UE_LOG(LogTemp, VeryVerbose, TEXT("  Triangle %d in Section %d: [%d->%d, %d->%d, %d->%d] Material %d"),
						ValidTrianglesInSection, SectionIdx,
						I0, OutOldToNewVertexMap[I0],
						I1, OutOldToNewVertexMap[I1],
						I2, OutOldToNewVertexMap[I2],
						Section.MaterialIndex);*/
				}
			}
			else
			{
				// Some vertices are missing - skip this triangle
				SkippedTrianglesInSection++;

				if (SkippedTrianglesInSection <= 3) // Only log first few to avoid spam
				{
					/*UE_LOG(LogTemp, VeryVerbose, TEXT("  Skipping triangle %d in Section %d: vertices [%d(%s), %d(%s), %d(%s)]"),
						TriIdx, SectionIdx,
						I0, bHasI0 ? TEXT("✓") : TEXT("✗"),
						I1, bHasI1 ? TEXT("✓") : TEXT("✗"),
						I2, bHasI2 ? TEXT("✓") : TEXT("✗"));*/
				}
			}
		}

		SectionTriangleCounts.Add(SectionIdx, ValidTrianglesInSection);
		SectionSkippedCounts.Add(SectionIdx, SkippedTrianglesInSection);

		/*UE_LOG(LogTemp, Warning, TEXT("  Section %d (Material %d): %d valid triangles, %d skipped"),
			SectionIdx, Section.MaterialIndex, ValidTrianglesInSection, SkippedTrianglesInSection);*/
	}

	// Final summary
	/*UE_LOG(LogTemp, Warning, TEXT("BuildStaticMeshData complete: %d triangles, %d material indices"),
		OutTriangles.Num(), OutMaterialIndices.Num());*/

	// Validate material indices
	TMap<int32, int32> MaterialIndexCounts;
	for (int32 MatIdx : OutMaterialIndices)
	{
		MaterialIndexCounts.FindOrAdd(MatIdx)++;
	}

	
}

// Add this debug to your BuildStaticMeshData to see which triangles are being extracted
void ULimbExtractorTool::DebugTriangleExtraction(
	USkeletalMesh* SkeletalMesh,
	const TArray<int32>& ExtractedVertexIndices,
	const TArray<FIntVector>& OutTriangles,
	const TArray<int32>& OutMaterialIndices)
{
	if (!SkeletalMesh) return;

	const FSkeletalMeshRenderData* RenderData = SkeletalMesh->GetResourceForRendering();
	if (!RenderData || RenderData->LODRenderData.Num() == 0) return;

	const FSkeletalMeshLODRenderData& LODData = RenderData->LODRenderData[0];

	UE_LOG(LogTemp, Warning, TEXT("=== TRIANGLE EXTRACTION ANALYSIS ==="));

	// Get material names for clarity
	TArray<FString> MaterialNames;
	for (const FSkeletalMaterial& Mat : SkeletalMesh->GetMaterials())
	{
		MaterialNames.Add(Mat.MaterialInterface ? Mat.MaterialInterface->GetName() : TEXT("NULL"));
	}

	// Count extracted triangles per material
	TMap<int32, int32> ExtractedTrianglesByMaterial;
	for (int32 MatIdx : OutMaterialIndices)
	{
		ExtractedTrianglesByMaterial.FindOrAdd(MatIdx)++;
	}

	// Count total triangles per section in original mesh
	TMap<int32, int32> OriginalTrianglesByMaterial;
	for (const FSkelMeshRenderSection& Section : LODData.RenderSections)
	{
		OriginalTrianglesByMaterial.Add(Section.MaterialIndex, Section.NumTriangles);
	}

	UE_LOG(LogTemp, Warning, TEXT("Triangle extraction comparison:"));
	for (int32 MatIdx = 0; MatIdx < MaterialNames.Num(); ++MatIdx)
	{
		int32 OriginalCount = OriginalTrianglesByMaterial.Contains(MatIdx) ? OriginalTrianglesByMaterial[MatIdx] : 0;
		int32 ExtractedCount = ExtractedTrianglesByMaterial.Contains(MatIdx) ? ExtractedTrianglesByMaterial[MatIdx] : 0;
		float Percentage = OriginalCount > 0 ? (float)ExtractedCount / (float)OriginalCount * 100.0f : 0.0f;

		FString MaterialName = MaterialNames.IsValidIndex(MatIdx) ? MaterialNames[MatIdx] : FString::Printf(TEXT("Mat_%d"), MatIdx);

		UE_LOG(LogTemp, Warning, TEXT("  %s (Mat %d): %d/%d triangles extracted (%.1f%%)"),
			*MaterialName, MatIdx, ExtractedCount, OriginalCount, Percentage);

		// Flag suspicious extractions
		if (Percentage > 95.0f && ExtractedCount > 100)
		{
			UE_LOG(LogTemp, Warning, TEXT("    ⚠️  Nearly all triangles extracted - might include boundary triangles"));
		}
		else if (Percentage > 0.0f && Percentage < 5.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("    ⚠️  Very few triangles extracted - might be missing geometry"));
		}
	}

	// Check for cross-material vertex sharing
	UE_LOG(LogTemp, Warning, TEXT("Checking for cross-material vertex influences..."));

	TSet<int32> ExtractedVertexSet(ExtractedVertexIndices);
	TMap<int32, TArray<int32>> MaterialVertexOverlaps; // Which materials share vertices

	const FRawStaticIndexBuffer16or32Interface* IndexBuffer = LODData.MultiSizeIndexContainer.GetIndexBuffer();

	for (const FSkelMeshRenderSection& Section : LODData.RenderSections)
	{
		const int32 BaseIndex = Section.BaseIndex;
		const int32 NumTris = Section.NumTriangles;
		int32 SharedVertexCount = 0;

		for (int32 TriIdx = 0; TriIdx < NumTris; ++TriIdx)
		{
			int32 I0 = IndexBuffer->Get(BaseIndex + TriIdx * 3 + 0);
			int32 I1 = IndexBuffer->Get(BaseIndex + TriIdx * 3 + 1);
			int32 I2 = IndexBuffer->Get(BaseIndex + TriIdx * 3 + 2);

			if (ExtractedVertexSet.Contains(I0) || ExtractedVertexSet.Contains(I1) || ExtractedVertexSet.Contains(I2))
			{
				SharedVertexCount++;
			}
		}

		if (SharedVertexCount > 0)
		{
			float SharePercentage = (float)SharedVertexCount / (float)NumTris * 100.0f;
			FString MaterialName = MaterialNames.IsValidIndex(Section.MaterialIndex) ?
				MaterialNames[Section.MaterialIndex] : FString::Printf(TEXT("Mat_%d"), Section.MaterialIndex);

			UE_LOG(LogTemp, Warning, TEXT("  %s: %d/%d triangles have extracted vertices (%.1f%%)"),
				*MaterialName, SharedVertexCount, NumTris, SharePercentage);

			if (SharePercentage > 0.0f && SharePercentage < 50.0f)
			{
				UE_LOG(LogTemp, Warning, TEXT("    🎯 POTENTIAL ISSUE: Partial vertex sharing might cause incorrect material assignment"));
			}
		}
	}
}

// Call this at the end of BuildStaticMeshData:
// DebugTriangleExtraction(SkeletalMesh, ExtractedVertexIndices, OutTriangles, OutMaterialIndices);

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
void ULimbExtractorTool::GenerateMaterialToTrianglesMap(
	const TArray<int32>& MaterialIndices,
	const TArray<int32>& FinalTriangles,
	TMap<int32, TArray<int32>>& OutMaterialToTriangles)
{
	OutMaterialToTriangles.Reset();

	const int32 NumTriangles = FinalTriangles.Num() / 3;
	const int32 NumMaterialIndices = MaterialIndices.Num();

	if (NumTriangles != NumMaterialIndices)
	{
		UE_LOG(LogTemp, Error, TEXT("MISMATCH: Triangle count (%d) != Material indices count (%d)"),
			NumTriangles, NumMaterialIndices);

		// Handle the mismatch gracefully
		const int32 SafeTriangleCount = FMath::Min(NumTriangles, NumMaterialIndices);
		UE_LOG(LogTemp, Warning, TEXT("Processing only %d triangles to avoid out-of-bounds access"),
			SafeTriangleCount);
	}

	// Track material distribution for debugging
	TMap<int32, int32> MaterialTriangleCounts;

	for (int32 TriangleIdx = 0; TriangleIdx < NumTriangles; ++TriangleIdx)
	{
		if (!MaterialIndices.IsValidIndex(TriangleIdx))
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing MaterialIndex for triangle %d"), TriangleIdx);
			continue;
		}

		// Validate triangle indices are within bounds
		int32 I0 = FinalTriangles[TriangleIdx * 3 + 0];
		int32 I1 = FinalTriangles[TriangleIdx * 3 + 1];
		int32 I2 = FinalTriangles[TriangleIdx * 3 + 2];

		int32 MaterialIndex = MaterialIndices[TriangleIdx];

		// Count triangles per material for debugging
		MaterialTriangleCounts.FindOrAdd(MaterialIndex)++;

		// Log suspicious material assignments (first few triangles only to avoid spam)
		/*if (TriangleIdx < 10)
		{
			UE_LOG(LogTemp, VeryVerbose, TEXT("Triangle %d: Material %d, Indices [%d, %d, %d]"),
				TriangleIdx, MaterialIndex, I0, I1, I2);
		}*/

		TArray<int32>& SectionList = OutMaterialToTriangles.FindOrAdd(MaterialIndex);
		SectionList.Add(I0);
		SectionList.Add(I1);
		SectionList.Add(I2);
	}

	// Debug output: show triangle distribution per material
	/*UE_LOG(LogTemp, Warning, TEXT("Triangle distribution by material:"));
	for (const auto& Pair : MaterialTriangleCounts)
	{
		UE_LOG(LogTemp, Warning, TEXT("  Material %d: %d triangles"), Pair.Key, Pair.Value);
	}*/

	// Additional validation: check for negative or suspiciously high material indices
	for (int32 TriangleIdx = 0; TriangleIdx < FMath::Min(NumTriangles, NumMaterialIndices); ++TriangleIdx)
	{
		int32 MaterialIndex = MaterialIndices[TriangleIdx];
		if (MaterialIndex < 0 || MaterialIndex > 20) // Assuming we don't have more than 20 materials
		{
			UE_LOG(LogTemp, Error, TEXT("Suspicious MaterialIndex %d for triangle %d"), MaterialIndex, TriangleIdx);
		}
	}
}

// Function to group open edges into edge loops
TArray<TArray<int32>> ULimbExtractorTool::SortOpenEdgesIntoLoops(const TSet<TPair<int32, int32>>& OpenEdges, bool bEnforceCCW)
{
	
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
void ULimbExtractorTool::GenerateCapFanFromLoop(const TArray<FVector3f>& Loop, const TArray<FVector3f>& FinalVertices, TArray<FVector3f>& CapVertices, TArray<int32>& CapTriangles,	TArray<FVector2D>& CapUVs,	TArray<FVector>& CapNormals)
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

// checker to see if a fadering is in danger of producing twisted or malformed results
bool ULimbExtractorTool::IsLoopTwistRisk(const TArray<FVector3f>& LoopPositions, float PlanarityThreshold, float MaxNormalDeviation, float SegmentSlopeTolerance, int32 SegmentWindow)
{
	const int32 Count = LoopPositions.Num();
	if (Count < 4)
	{
		UE_LOG(LogTemp, Warning, TEXT("Twist check skipped: not enough loop positions (%d)"), Count);
		return false;
	}

	// Compute loop center
	FVector3f Center(0);
	for (const FVector3f& V : LoopPositions)
	{
		Center += V;
	}
	Center /= float(Count);

	// Step 1: Compute loop normal
	const FVector3f LoopNormal = ComputeLoopNormalWithTolerance(LoopPositions, PlanarityThreshold);
	if (LoopNormal.IsNearlyZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("Twist check aborted: loop normal is degenerate."));
		return false;
	}

	// Step 2: Compute segment normals
	TArray<FVector3f> SegmentNormals;
	SegmentNormals.Reserve(Count);

	for (int32 i = 0; i < Count; ++i)
	{
		const FVector3f& Prev = LoopPositions[(i - 1 + Count) % Count];
		const FVector3f& Curr = LoopPositions[i];
		const FVector3f& Next = LoopPositions[(i + 1) % Count];

		const FVector3f TangentDir = (Next - Prev).GetSafeNormal();
		const FVector3f ToCenter = (Center - Curr).GetSafeNormal();
		const FVector3f Normal = FVector3f::CrossProduct(TangentDir, ToCenter).GetSafeNormal();

		SegmentNormals.Add(Normal);
	}

	// Step 3: Count twist violations
	int32 TwistCount = 0;
	for (const FVector3f& N : SegmentNormals)
	{
		const float Dot = FVector3f::DotProduct(N.GetSafeNormal(), LoopNormal);
		if (FMath::Abs(Dot) < MaxNormalDeviation)
		{
			TwistCount++;
		}
	}

	// Step 4: Segment slope check
	auto SafeIndex = [Count](int32 i) { return (i % Count + Count) % Count; };

	int32 SlopeViolations = 0;
	for (int32 i = 0; i < Count; ++i)
	{
		const FVector3f Curr = SegmentNormals[i];
		const FVector3f Prev = SegmentNormals[SafeIndex(i - SegmentWindow)];
		const FVector3f Next = SegmentNormals[SafeIndex(i + SegmentWindow)];

		const float AnglePrev = FMath::RadiansToDegrees(FMath::Acos(
			FMath::Clamp(FVector3f::DotProduct(Curr.GetSafeNormal(), Prev.GetSafeNormal()), -1.f, 1.f)));

		const float AngleNext = FMath::RadiansToDegrees(FMath::Acos(
			FMath::Clamp(FVector3f::DotProduct(Curr.GetSafeNormal(), Next.GetSafeNormal()), -1.f, 1.f)));

		if (AnglePrev > SegmentSlopeTolerance * 45.f || AngleNext > SegmentSlopeTolerance * 45.f)
		{
			SlopeViolations++;
		}
	}

	// Step 5: Compute average radius
	float Radius = 0.f;
	for (const FVector3f& V : LoopPositions)
	{
		Radius += FVector3f::Distance(V, Center);
	}
	Radius /= float(Count);

	// Step 6: Compute tolerances based on scale
	const float TwistRatioTolerance = Radius < 5.f ? 0.6f : 0.4f;
	const float SlopeRatioTolerance = Radius < 5.f ? 0.5f : 0.3f;

	// Step 7: Final decision
	const float TwistRatio = float(TwistCount) / float(Count);
	const float SlopeRatio = float(SlopeViolations) / float(Count);
	const bool bTwistRisk = (TwistRatio > TwistRatioTolerance) || (SlopeRatio > SlopeRatioTolerance);

	UE_LOG(LogTemp, Warning, TEXT("TwistCheck: Radius=%.2f, TwistRatio=%.2f, SlopeRatio=%.2f, Tolerance=[%.2f, %.2f]"),
		Radius, TwistRatio, SlopeRatio, TwistRatioTolerance, SlopeRatioTolerance);

	UE_LOG(LogTemp, Verbose, TEXT("Twist detection: SegmentNormals=%d, TwistRatio=%.2f, SlopeRatio=%.2f, Risk=%s"),
		Count, TwistRatio, SlopeRatio, bTwistRisk ? TEXT("✅") : TEXT("❌"));

	return bTwistRisk;
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
	float T = (Ring - .5) / float(RingCount - .5);  // ensure full span
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
	
	FVector3f Normal = ComputeLoopNormalPCA(BaseLoopPositions);/*ComputeLoopNormalWithTolerance(BaseLoopPositions, 1.0f);*/// was 1.5
	FVector3f P0 = BaseLoopPositions[0];
	FVector3f P1 = BaseLoopPositions[LoopSize / 2];
	FVector3f LimbAxis = (P1 - P0).GetSafeNormal();
	FVector3f Tangent = (BaseLoopPositions[1] - P0).GetSafeNormal();
	FVector3f SliceNormal = FVector3f::CrossProduct(LimbAxis, Tangent).GetSafeNormal() * -1;
	
	GetNormalFromBoneAxis(LoopInfo, SliceNormal);

	//DrawLoopNormalDebug(BaseLoopPositions, Normal, FColor::Green, 100.0f, 5.0f);

	DrawLoopNormalDebug(BaseLoopPositions, SliceNormal, FColor::Green, 100.0f, 5.0f);

	// Step 2: Estimate average radius
	float AvgRadius = 0.f;
	for (const FVector3f& V : BaseLoopPositions)
	{
		AvgRadius += FVector3f::Dist(V, Center);
	}
	AvgRadius /= LoopSize;
	float Radius = AvgRadius * 0.95f;

	// Step 3: Add base ring verts
	TArray<int32> BaseRing;
	TArray<FVector2D> BaseUVs;

	for (int32 i = 0; i < LoopSize; ++i)
	{
		int32 Idx = OutVertices.Add(BaseLoopPositions[i]);

		int32 GlobalIndex = VertexIndices[i];
		FVector2D UV = CachedMeshContext.FinalUVs.IsValidIndex(GlobalIndex)	? CachedMeshContext.FinalUVs[GlobalIndex] : FVector2D(0.5f, 0.5f);

		OutUVs.Add(UV);
		OutNormals.Add(FVector(SliceNormal));
		BaseRing.Add(Idx);
		BaseUVs.Add(UV); // ✅ store for reuse


	}

	// Step 4: Build tangent basis
	FVector3f X = (BaseLoopPositions[0] - Center).GetSafeNormal();
	FVector3f Y = FVector3f::CrossProduct(SliceNormal, X).GetSafeNormal();

	
	if (LoopInfo.bFlatten)// || bFlattenToPlane)
	{
		// Step 5: Create fade ring with outward projection and planar flatten
		for (int32 i = 0; i < LoopSize; ++i)
		{
			const FVector3f& BasePos = BaseLoopPositions[i];
			FVector3f Offset = BasePos - Center;

			// Project outward by scale factor (e.g. 1.05 = 5% larger)
			const float Scale = .95f;
			FVector3f ScaledOffset = Offset * Scale;
			FVector3f Pos = Center + ScaledOffset;

			// Flatten to slice plane
			FVector3f Flattened = Center + FVector3f::DotProduct(ScaledOffset, X) * X
				+ FVector3f::DotProduct(ScaledOffset, Y) * Y;

			if (LoopInfo.bAddNoise)
			{
				float Noise = FMath::FRandRange(-LoopInfo.NoiseAmount, LoopInfo.NoiseAmount);
				Flattened += SliceNormal * Noise;
			}

			int32 FadeIdx = OutVertices.Add(Flattened);
			OutUVs.Add(BaseUVs.IsValidIndex(i) ? BaseUVs[i] : FVector2D(0.5f, 0.5f));
			OutNormals.Add(FVector(SliceNormal));
			OutFadeRingIndices.Add(FadeIdx);
		}
	}
	else // it'll be just a circular projection
	{
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
	}
	

	// Step 6: Push fade ring outward  *** SliceNormal is the key to the correct direction ***

	FVector3f PushDir = SliceNormal * LoopInfo.FadeRingHieght;
	
	for (int32 Index : OutFadeRingIndices)
	{
		OutVertices[Index] += PushDir;
	}

	
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

void ULimbExtractorTool::GetNormalFromBoneAxis(const FEdgeLoopInfo& LoopInfo, FVector3f& SliceNormal)
{
	if (!LoopInfo.AssociatedBone.IsEmpty() && InMesh && LoopInfo.bUseManualNormal)
	{
		FString Name = LoopInfo.AssociatedBone.ToString();
		FName Bone = FName(Name);
		const FReferenceSkeleton& RefSkeleton = InMesh->GetRefSkeleton();
		const int32 BoneIndex = RefSkeleton.FindBoneIndex(Bone);

		if (BoneIndex != INDEX_NONE)
		{
			// This gives you the bone's transform in component space (reference pose)
			const FTransform BoneTransform = RefSkeleton.GetRefBonePose()[BoneIndex];

			// You can now extract axis vectors
			FVector BoneLocation = BoneTransform.GetLocation();
			FVector BoneForward = BoneTransform.GetRotation().GetForwardVector(); // X axis
			FVector BoneUp = BoneTransform.GetRotation().GetUpVector();           // Z axis
			FVector BoneRight = BoneTransform.GetRotation().GetRightVector();     // Y axis

			SliceNormal = FVector3f(BoneForward);
			// Use BoneForward or BoneUp as your normal or limb axis
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Bone '%s' not found in skeleton."), *LoopInfo.AssociatedBone.ToString());
		}

	}
}

// Computes the best‐fit plane normal for a point loop using PCA (smallest covariance eigenvector).
// Falls back to Newell’s method if the covariance matrix is singular.
FVector3f ULimbExtractorTool::ComputeLoopNormalPCA(const TArray<FVector3f>& LoopVerts)
{
	const int32 N = LoopVerts.Num();
	if (N < 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeLoopNormalPCA: Need at least 3 verts."));
		return FVector3f::UpVector;
	}

	// 1) Compute centroid
	FVector3f Mean = FVector3f::ZeroVector;
	for (const FVector3f& V : LoopVerts)
	{
		Mean += V;
	}
	Mean /= float(N);

	// 2) Build (unnormalized) covariance matrix entries
	float cxx = 0, cxy = 0, cxz = 0;
	float cyy = 0, cyz = 0, czz = 0;
	for (const FVector3f& V : LoopVerts)
	{
		FVector3f D = V - Mean;
		cxx += D.X * D.X;
		cxy += D.X * D.Y;
		cxz += D.X * D.Z;
		cyy += D.Y * D.Y;
		cyz += D.Y * D.Z;
		czz += D.Z * D.Z;
	}

	// 3) Invert the 3×3 covariance (for power‐iteration of smallest eigenvector)
	//    det = m00*m11*m22 + 2*m01*m02*m12 - m00*m12^2 - m11*m02^2 - m22*m01^2
	const float m00 = cxx, m01 = cxy, m02 = cxz;
	const float m11 = cyy, m12 = cyz, m22 = czz;
	float det = m00 * m11 * m22
		+ 2.0f * m01 * m02 * m12
		- m00 * m12 * m12
		- m11 * m02 * m02
		- m22 * m01 * m01;

	if (FMath::IsNearlyZero(det))
	{
		// Fallback: Newell’s polygon normal
		FVector3f Rough(0);
		for (int32 i = 0; i < N; ++i)
		{
			const FVector3f& A = LoopVerts[i];
			const FVector3f& B = LoopVerts[(i + 1) % N];
			Rough.X += (A.Y - B.Y) * (A.Z + B.Z);
			Rough.Y += (A.Z - B.Z) * (A.X + B.X);
			Rough.Z += (A.X - B.X) * (A.Y + B.Y);
		}
		return Rough.GetSafeNormal();
	}

	const float invDet = 1.0f / det;
	// Adjugate matrix for inverse
	float i00 = (m11 * m22 - m12 * m12) * invDet;
	float i01 = (m02 * m12 - m01 * m22) * invDet;
	float i02 = (m01 * m12 - m02 * m11) * invDet;
	float i11 = (m00 * m22 - m02 * m02) * invDet;
	float i12 = (m01 * m02 - m00 * m12) * invDet;
	float i22 = (m00 * m11 - m01 * m01) * invDet;

	// 4) Power‐iteration to find dominant eigenvector of (Covariance)^-1 ⇒ smallest of Cov
	FVector3f Normal(1, 1, 1);
	Normal.Normalize();
	for (int32 iter = 0; iter < 10; ++iter)
	{
		FVector3f Next;
		Next.X = i00 * Normal.X + i01 * Normal.Y + i02 * Normal.Z;
		Next.Y = i01 * Normal.X + i11 * Normal.Y + i12 * Normal.Z;
		Next.Z = i02 * Normal.X + i12 * Normal.Y + i22 * Normal.Z;
		Normal = Next.GetSafeNormal();
	}

	return Normal;
}

void ULimbExtractorTool::GenerateRobustFadeRingCapFromLoopInfo
(
	const FEdgeLoopInfo& LoopInfo,
	TArray<FVector3f>& OutVertices,
	TArray<int32>& OutTriangles,
	TArray<FVector2D>& OutUVs,
	TArray<FVector>& OutNormals,
	TArray<FProcMeshTangent>& OutTangents,
	TArray<FLinearColor>& OutVertexColors,
	TArray<int32>& OutFadeRingIndices,
	TArray<int32>& OutFadeRingTriangles
)
{
	const TArray<FVector3f>& BaseLoopPositions = LoopInfo.Positions;
	const TArray<int32>& VertexIndices = LoopInfo.VertexIndices;

	if (BaseLoopPositions.Num() < 3) return;
	const int32 LoopSize = BaseLoopPositions.Num();

	OutFadeRingIndices.Reset();

	// Step 1: Compute center and normal
	FVector3f Center(0);
	for (const FVector3f& V : BaseLoopPositions)
		Center += V;
	Center /= LoopSize;

	FVector3f Normal = ComputeLoopNormalPCA(BaseLoopPositions);
	FVector3f P0 = BaseLoopPositions[0];
	FVector3f P1 = BaseLoopPositions[LoopSize / 2];
	FVector3f LimbAxis = (P1 - P0).GetSafeNormal();
	FVector3f Tangent = (BaseLoopPositions[1] - P0).GetSafeNormal();
	FVector3f SliceNormal = FVector3f::CrossProduct(LimbAxis, Tangent).GetSafeNormal() * -1;

	// Build tangent basis
	FVector3f X = (BaseLoopPositions[0] - Center).GetSafeNormal();
	FVector3f Y = FVector3f::CrossProduct(SliceNormal, X).GetSafeNormal();
	X = FVector3f::CrossProduct(Y, SliceNormal).GetSafeNormal();

	// Pre-calculate tangent direction for consistent orientation
	FVector3f BaseTangentDir = FVector3f::CrossProduct(SliceNormal, LimbAxis).GetSafeNormal();

	// Step 2: Twist risk flag
	const bool bTwistRisk = true;

	// Step 3: Estimate radius
	float AvgRadius = 0.f;
	for (const FVector3f& V : BaseLoopPositions)
		AvgRadius += FVector3f::Distance(V, Center);
	AvgRadius /= LoopSize;
	float Radius = AvgRadius * 0.95f;

	// Step 4: Add base ring
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
		OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));
		OutVertexColors.Add(FLinearColor::White);

		BaseRing.Add(Idx);
		BaseUVs.Add(UV);
	}

	// --- Step A: Precompute final pushed fade ring ---
	TArray<FVector3f> TargetRingPositions;
	TargetRingPositions.SetNum(LoopSize);

	FVector3f PushDir = SliceNormal * LoopInfo.FadeRingHieght;

	for (int32 i = 0; i < LoopSize; ++i)
	{
		FVector3f BasePos = BaseLoopPositions[i];
		FVector3f ToCenter = (Center - BasePos).GetSafeNormal();

		// Collapse toward center by LimbEndCollapseAmount
		FVector3f CollapsedPos = FMath::Lerp(BasePos, Center, LoopInfo.LimbEndCollapseAmount);

		// Add optional noise
		if (LoopInfo.bAddNoise)
		{
			float Noise = FMath::FRandRange(-LoopInfo.NoiseAmount, LoopInfo.NoiseAmount);
			CollapsedPos += ToCenter * Noise;
		}

		// Apply outward push immediately
		TargetRingPositions[i] = CollapsedPos + PushDir;
	}

	// Step 6: Generate intermediate rings (between base and pushed target)
	int32 RingCount = LoopInfo.FadeRingCount > 0 ? LoopInfo.FadeRingCount : 3;
	UE_LOG(LogTemp, Warning, TEXT("RingCount is %d"), RingCount);

	TArray<int32> PrevRing = BaseRing;

	for (int32 RingIndex = 0; RingIndex <= RingCount; ++RingIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("RingIndex is %d"), RingIndex);

		float T = (RingIndex + 1.0f) / float(RingCount + 1);

		TArray<int32> CurrRing;
		for (int32 i = 0; i < LoopSize; ++i)
		{
			// Interpolate between base and pushed target
			FVector3f Pos = FMath::Lerp(BaseLoopPositions[i], TargetRingPositions[i], T);

			// Per-point normal
			FVector3f LocalNormalb = bTwistRisk && LoopSize >= 3
				? FVector3f::CrossProduct(BaseLoopPositions[(i + 1) % LoopSize] - BaseLoopPositions[(i - 1 + LoopSize) % LoopSize],
					BaseLoopPositions[i] - Center).GetSafeNormal()
				: SliceNormal;

			int32 Idx = OutVertices.Add(Pos);

			// Interpolate UV from base ring
			FVector2D UV = BaseUVs[i];
			OutUVs.Add(UV);
			OutNormals.Add(FVector(LocalNormalb));
			OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));

			float Alpha = FMath::Lerp(1.0f, 0.3f, T);
			OutVertexColors.Add(FLinearColor(1.0f, 1.0f, 1.0f, Alpha));

			CurrRing.Add(Idx);
		}

		// Stitch previous ring to current ring (keeps seam handling)
		for (int32 i = 0; i < LoopSize; ++i)
		{
			int32 A = PrevRing[i];
			int32 B = PrevRing[(i + 1) % LoopSize];
			int32 C = CurrRing[i];
			int32 D = CurrRing[(i + 1) % LoopSize];

			if (i == 0)
			{
				// Manual override for seam
				FVector3f PosE = OutVertices[A];
				FVector3f PosF = OutVertices[B];
				FVector3f PosG = OutVertices[C];
				FVector3f PosH = OutVertices[D];

				FVector2D UV = OutUVs[A];
				FVector Normal2 = OutNormals[A];

				int32 NewE = OutVertices.Add(PosE);
				int32 NewF = OutVertices.Add(PosF);
				int32 NewG = OutVertices.Add(PosG);
				int32 NewH = OutVertices.Add(PosH);

				OutUVs.Add(UV); OutUVs.Add(UV); OutUVs.Add(UV); OutUVs.Add(UV);
				OutNormals.Add(Normal2); OutNormals.Add(Normal2); OutNormals.Add(Normal2); OutNormals.Add(Normal2);
				OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));
				OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));
				OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));
				OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));
				OutVertexColors.Add(FLinearColor::White);
				OutVertexColors.Add(FLinearColor::White);
				OutVertexColors.Add(FLinearColor::White);
				OutVertexColors.Add(FLinearColor::White);

				OutTriangles.Add(NewE); OutTriangles.Add(NewH); OutTriangles.Add(NewG);
				OutTriangles.Add(NewE); OutTriangles.Add(NewF); OutTriangles.Add(NewH);
				OutFadeRingTriangles.Append({ NewE, NewH, NewG, NewE, NewF, NewH });

				continue;
			}

			OutTriangles.Add(A); OutTriangles.Add(D); OutTriangles.Add(C);
			OutTriangles.Add(A); OutTriangles.Add(B); OutTriangles.Add(D);
			OutFadeRingTriangles.Add(A); OutFadeRingTriangles.Add(D); OutFadeRingTriangles.Add(C);
			OutFadeRingTriangles.Add(A); OutFadeRingTriangles.Add(B); OutFadeRingTriangles.Add(D);
		}

		PrevRing = CurrRing;
	}

	// Step 7: Add final pushed fade ring
	for (int32 i = 0; i < LoopSize; ++i)
	{
		FVector3f Pos = TargetRingPositions[i];

		FVector3f LocalNormalb = bTwistRisk && LoopSize >= 3
			? FVector3f::CrossProduct(BaseLoopPositions[(i + 1) % LoopSize] - BaseLoopPositions[(i - 1 + LoopSize) % LoopSize],
				BaseLoopPositions[i] - Center).GetSafeNormal()
			: SliceNormal;

		int32 FadeIdx = OutVertices.Add(Pos);

		FVector2D UV = BaseUVs[i];
		OutUVs.Add(UV);
		OutNormals.Add(FVector(LocalNormalb));
		OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));
		OutVertexColors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 0.1f));

		OutFadeRingIndices.Add(FadeIdx);
	}

	// Step 8: Stitch last intermediate ring to final fade ring (same as before)
	for (int32 i = 0; i < LoopSize; ++i)
	{
		int32 A = PrevRing[i];
		int32 B = PrevRing[(i + 1) % LoopSize];
		int32 C = OutFadeRingIndices[i];
		int32 D = OutFadeRingIndices[(i + 1) % LoopSize];

		if (i == 0)
		{
			FVector3f PosE = OutVertices[A];
			FVector3f PosF = OutVertices[B];
			FVector3f PosG = OutVertices[C];
			FVector3f PosH = OutVertices[D];

			FVector2D UV = OutUVs[A];
			FVector Normal2 = OutNormals[A];

			int32 NewE = OutVertices.Add(PosE);
			int32 NewF = OutVertices.Add(PosF);
			int32 NewG = OutVertices.Add(PosG);
			int32 NewH = OutVertices.Add(PosH);

			OutUVs.Add(UV); OutUVs.Add(UV); OutUVs.Add(UV); OutUVs.Add(UV);
			OutNormals.Add(Normal2); OutNormals.Add(Normal2); OutNormals.Add(Normal2); OutNormals.Add(Normal2);
			OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));
			OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));
			OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));
			OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));
			OutVertexColors.Add(FLinearColor::White);
			OutVertexColors.Add(FLinearColor::White);
			OutVertexColors.Add(FLinearColor::White);
			OutVertexColors.Add(FLinearColor::White);

			OutTriangles.Add(NewE); OutTriangles.Add(NewH); OutTriangles.Add(NewG);
			OutTriangles.Add(NewE); OutTriangles.Add(NewF); OutTriangles.Add(NewH);
			OutFadeRingTriangles.Append({ NewE, NewH, NewG, NewE, NewF, NewH });

			continue;
		}

		OutTriangles.Add(A); OutTriangles.Add(D); OutTriangles.Add(C);
		OutTriangles.Add(A); OutTriangles.Add(B); OutTriangles.Add(D);
		OutFadeRingTriangles.Add(A); OutFadeRingTriangles.Add(D); OutFadeRingTriangles.Add(C);
		OutFadeRingTriangles.Add(A); OutFadeRingTriangles.Add(B); OutFadeRingTriangles.Add(D);
	}

	// Generate flow-based fade UVs
	GenerateFlowBasedFadeUVs(BaseLoopPositions, BaseUVs, OutFadeRingIndices, OutUVs, OutVertices, SliceNormal);

	// Step 9 (old push step) removed, rings are already interpolated to pushed target

	// Step 10: Clean triangles
	TArray<FIntVector> FadeTris;
	for (int32 i = 0; i < OutFadeRingTriangles.Num(); i += 3)
		FadeTris.Add(FIntVector(OutFadeRingTriangles[i], OutFadeRingTriangles[i + 1], OutFadeRingTriangles[i + 2]));

	UMeshCleanUtils::RemoveDegenerateTriangles(FadeTris);
	UMeshCleanUtils::RemoveDuplicateTriangles(FadeTris);
	UMeshCleanUtils::RemoveNonManifoldTriangles(FadeTris);

	OutFadeRingTriangles.Reset();
	for (const FIntVector& Tri : FadeTris)
	{
		OutFadeRingTriangles.Add(Tri.X);
		OutFadeRingTriangles.Add(Tri.Y);
		OutFadeRingTriangles.Add(Tri.Z);
	}
}

// 
void ULimbExtractorTool::GenerateReverseFadeRingFromLoopInfo(
	const FEdgeLoopInfo& LoopInfo,
	TArray<FVector3f>& OutVertices,
	TArray<int32>& OutTriangles,
	TArray<FVector2D>& OutUVs,
	TArray<FVector>& OutNormals,
	TArray<FProcMeshTangent> OutTangents,
	TArray<FLinearColor> OutVertexColors,
	TArray<int32>& OutFadeRingIndices,
	TArray<int32>& OutFadeRingTriangles
)
{
	const TArray<FVector3f>& BaseLoopPositions = LoopInfo.Positions;
	const TArray<int32>& VertexIndices = LoopInfo.VertexIndices;

	if (BaseLoopPositions.Num() < 3) return;
	const int32 LoopSize = BaseLoopPositions.Num();

	OutFadeRingIndices.Reset();
	
	// Step 1: Compute center and slice normal
	FVector3f Center(0);
	for (const FVector3f& V : BaseLoopPositions)
		Center += V;
	Center /= LoopSize;

	FVector3f P0 = BaseLoopPositions[0];
	FVector3f P1 = BaseLoopPositions[LoopSize / 2];
	FVector3f LimbAxis = (P1 - P0).GetSafeNormal();
	FVector3f Tangent = (BaseLoopPositions[1] - P0).GetSafeNormal();
	FVector3f SliceNormal = FVector3f::CrossProduct(LimbAxis, Tangent).GetSafeNormal();

	//Pre-calculate tangent direction for consistent orientation
	FVector3f BaseTangentDir = FVector3f::CrossProduct(SliceNormal, LimbAxis).GetSafeNormal();

	// Step 2: Add base ring (untouched)
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
		BaseUVs.Add(UV);
		// Add tangent for base ring
		OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));

		// Add vertex color for base ring (white/opaque)
		OutVertexColors.Add(FLinearColor::White);
	}

	// Step 3: Generate outer ring with flare and push
	TArray<int32> OuterRing;
	for (int32 i = 0; i < LoopSize; ++i)
	{
		const FVector3f V = BaseLoopPositions[i];
		FVector3f FlareDir = (V - Center).GetSafeNormal();

		// Apply flare and push
		FVector3f Pushed = V + SliceNormal * LoopInfo.ReverseCapFadeAmount + FlareDir * LoopInfo.RevCapExpansionAmount;

		int32 Idx = OutVertices.Add(Pushed);
		OutUVs.Add(BaseUVs[i]);
		OutNormals.Add(FVector(SliceNormal));
		OuterRing.Add(Idx);

		// Add tangent for outer ring
		OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));

		// Add vertex color for outer ring (slightly transparent for fade effect)
		OutVertexColors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 0.8f));

		//DrawDebugPoint(GetWorld(), FVector(OuterRing[i]), 10.0f, FColor::Blue, false, 10.0f);

	}

	// Step 4: Stitch base ring to outer ring
	for (int32 i = 0; i < LoopSize; ++i)
	{
		int32 A = BaseRing[i];
		int32 B = BaseRing[(i + 1) % LoopSize];
		int32 C = OuterRing[i];
		int32 D = OuterRing[(i + 1) % LoopSize];

		OutTriangles.Add(A); OutTriangles.Add(D); OutTriangles.Add(C);
		OutTriangles.Add(A); OutTriangles.Add(B); OutTriangles.Add(D);

		OutFadeRingTriangles.Add(A); OutFadeRingTriangles.Add(D); OutFadeRingTriangles.Add(C);
		OutFadeRingTriangles.Add(A); OutFadeRingTriangles.Add(B); OutFadeRingTriangles.Add(D);
	}

	for (int32 i = 0; i < LoopSize; ++i)
	{
		const FVector3f& Original = BaseLoopPositions[i];
		const FVector3f& Final = OutVertices[BaseRing[i]];
		float Offset = FVector3f::Distance(Original, Final);
		if (Offset > KINDA_SMALL_NUMBER)
		{
			UE_LOG(LogTemp, Warning, TEXT("BaseRing[%d] has been modified! Offset: %.4f"), i, Offset);
		}
	}

	// Step 5: Generate fade rings between base and outer ring
	int32 RingCount = LoopInfo.RevCapFadeRingCount > 0 ? LoopInfo.RevCapFadeRingCount : 3;
	TArray<int32> PrevRing = BaseRing;

	for (int32 RingIndex = 0; RingIndex <= RingCount; ++RingIndex)
	{
		float T = (RingIndex + 0.5f) / float(RingCount);
		float FlareStrength = FMath::Sin(T * PI * 0.5f) * LoopInfo.RevCapExpansionAmount;

		TArray<int32> CurrRing;
		for (int32 i = 0; i < LoopSize; ++i)
		{
			const FVector3f A = OutVertices[BaseRing[i]];
			const FVector3f B = OutVertices[OuterRing[i]];

			FVector3f Pos = FMath::Lerp(A, B, T);

			// Apply flare outward from center
			FVector3f FlareDir = (Pos - Center).GetSafeNormal();
			Pos += FlareDir * FlareStrength;

			// Apply radial noise
			if (LoopInfo.bAddNoise)
			{
				float Noise = FMath::FRandRange(-LoopInfo.NoiseAmount, LoopInfo.NoiseAmount);
				Pos += FlareDir * Noise;
			}

			int32 Idx = OutVertices.Add(Pos);
			OutUVs.Add(BaseUVs[i]);
			OutNormals.Add(FVector(SliceNormal));
			// Add tangent for intermediate ring
			OutTangents.Add(FProcMeshTangent(FVector(BaseTangentDir), false));

			// Add vertex color with progressive fade based on ring position
			float Alpha = FMath::Lerp(1.0f, 0.2f, T); // Fade from opaque to more transparent
			OutVertexColors.Add(FLinearColor(1.0f, 1.0f, 1.0f, Alpha));

			CurrRing.Add(Idx);
		}

		// Stitch previous ring to current ring
		for (int32 i = 0; i < LoopSize; ++i)
		{
			int32 A = PrevRing[i];
			int32 B = PrevRing[(i + 1) % LoopSize];
			int32 C = CurrRing[i];
			int32 D = CurrRing[(i + 1) % LoopSize];

			OutTriangles.Add(A); OutTriangles.Add(D); OutTriangles.Add(C);
			OutTriangles.Add(A); OutTriangles.Add(B); OutTriangles.Add(D);

			OutFadeRingTriangles.Add(A); OutFadeRingTriangles.Add(D); OutFadeRingTriangles.Add(C);
			OutFadeRingTriangles.Add(A); OutFadeRingTriangles.Add(B); OutFadeRingTriangles.Add(D);
		/*	DrawDebugPoint(GetWorld(), FVector(OutVertices[BaseRing[i]]), 10.0f, FColor::Green, false, 10.0f);
			DrawDebugPoint(GetWorld(), FVector(OutVertices[PrevRing[i]]), 10.0f, FColor::Orange, false, 10.0f);

			DrawDebugPoint(GetWorld(), FVector(OutVertices[CurrRing[i]]), 10.0f, FColor::Green, false, 10.0f);*/

		}

		PrevRing = CurrRing;
	}



	// Step 6: Apply noise to inner rings only
	for (int32 i = BaseRing.Num(); i < OutVertices.Num(); ++i)
	{
		FVector3f& V = OutVertices[i];
		FVector3f ToCenter = (Center - V).GetSafeNormal();
		FVector3f OutwardDir = -ToCenter;

		if (LoopInfo.bAddNoise)
		{
			float Noise = FMath::FRandRange(-LoopInfo.NoiseAmount, LoopInfo.NoiseAmount);
			V += OutwardDir * Noise;
		}
	}
	//GenerateFlowBasedFadeUVs(BaseLoopPositions, BaseUVs, OutFadeRingIndices, OutUVs, OutVertices, SliceNormal);
	ProjectPlanarUVsForCap(OutVertices, 0, OutVertices.Num() - 1, FVector3f(Center), SliceNormal, OutUVs, FVector2d(BP_RCFRUVScale), BP_RCFRUVRotationDegrees, BP_RCFRUVOffset);
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
	{
		Mean += D;
	}
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

void ULimbExtractorTool::DrawLoopNormalDebug(
	const TArray<FVector3f>& LoopVerts,
	const FVector3f& Normal,
	FColor Color,
	float Length = 100.f,
	float Duration = 5.f)
{
	if (LoopVerts.Num() < 1) return;

	// Compute loop center
	FVector3f Center(0);
	for (const FVector3f& V : LoopVerts)
		Center += V;
	Center /= LoopVerts.Num();

	// Draw line
	const FVector3f End = Center + Normal * Length;
	DrawDebugLine(
		GetWorld(),
		FVector(Center),
		FVector(End),
		Color,
		false,
		Duration,
		0,
		1.5f
	);
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

// Method 1: UV Flow Extension - Extends UVs along the natural flow direction
void ULimbExtractorTool::GenerateFlowBasedFadeUVs(
	const TArray<FVector3f>& BaseLoopPositions,
	const TArray<FVector2D>& BaseUVs,
	const TArray<int32>& OutFadeRingIndices,
	TArray<FVector2D>& OutUVs,
	const TArray<FVector3f>& OutVertices,
	FVector3f SliceNormal)
{
	// Calculate UV flow directions around the loop
	TArray<FVector2D> UVFlowDirections;
	UVFlowDirections.SetNum(BaseUVs.Num());

	for (int32 i = 0; i < BaseUVs.Num(); ++i)
	{
		int32 PrevIdx = (i - 1 + BaseUVs.Num()) % BaseUVs.Num();
		int32 NextIdx = (i + 1) % BaseUVs.Num();

		// Get UV tangent along the loop
		FVector2D UVTangent = (BaseUVs[NextIdx] - BaseUVs[PrevIdx]).GetSafeNormal();

		// Get 3D positions for comparison
		FVector3f Prev3D = BaseLoopPositions[PrevIdx];
		FVector3f Next3D = BaseLoopPositions[NextIdx];
		FVector3f Tangent3D = (Next3D - Prev3D).GetSafeNormal();

		// Calculate perpendicular direction in UV space
		// This should point "inward" toward where the fade ring goes
		FVector2D UVPerpendicular(-UVTangent.Y, UVTangent.X);

		// Determine if we need to flip based on 3D geometry
		FVector3f ToCenter = (BaseLoopPositions[0] - BaseLoopPositions[i]).GetSafeNormal();
		FVector3f Cross = FVector3f::CrossProduct(Tangent3D, ToCenter);
		float Orientation = FVector3f::DotProduct(Cross, SliceNormal);

		if (Orientation < 0)
		{
			UVPerpendicular *= -1.0f;
		}

		UVFlowDirections[i] = UVPerpendicular;
	}

	// Generate fade ring UVs by extending along flow direction
	for (int32 i = 0; i < OutFadeRingIndices.Num(); ++i)
	{
		int32 FadeIdx = OutFadeRingIndices[i];

		// Calculate how far we moved in 3D
		FVector3f BasePos = BaseLoopPositions[i];
		FVector3f FadePos = OutVertices[FadeIdx];
		float MovementDistance = FVector3f::Distance(FadePos, BasePos);

		// Estimate UV scale (this might need tuning per mesh)

		float UVScale = MovementDistance / EstimateWorldToUVScale(BaseLoopPositions, BaseUVs);

		// Extend UV along flow direction
		FVector2D BaseUV = BaseUVs[i];
		FVector2D FlowDir = UVFlowDirections[i];
		FVector2D FadeUV = BaseUV + FlowDir * UVScale;

		OutUVs[FadeIdx] = FadeUV;
	}
}

float ULimbExtractorTool::EstimateWorldToUVScale(const TArray<FVector3f>& WorldPositions, const TArray<FVector2D>& UVs)
{
	float TotalWorldDist = 0.0f;
	float TotalUVDist = 0.0f;

	for (int32 i = 0; i < WorldPositions.Num(); ++i)
	{
		int32 NextIdx = (i + 1) % WorldPositions.Num();

		float WorldDist = FVector3f::Distance(WorldPositions[i], WorldPositions[NextIdx]);
		float UVDist = FVector2D::Distance(UVs[i], UVs[NextIdx]);

		TotalWorldDist += WorldDist;
		TotalUVDist += UVDist;
	}

	return TotalUVDist > SMALL_NUMBER ? TotalWorldDist / TotalUVDist : 100.0f;
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


