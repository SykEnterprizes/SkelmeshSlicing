///////// Copyright to Stacey Denman of SykProductions 2025

#pragma once
#include "CoreMinimal.h"
#include </Program Files/Epic Games/UE_5.0/Engine/Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h>
#include "BoneHierachyStruct.h"

#include "FRawVertexData.h"

#include "FMeshBuildContext.h"
#include "FEdgeLoopInfo.h"
#include "MeshDescription.h"
#include "StaticMeshAttributes.h"
#include "MeshDescriptionBuilder.h"
#include "LimbMeshCleaner.h"
#include "UEdgeLoopClusteringUtility.h"
#include "ULimbExtractorTool.generated.h"

USTRUCT(BlueprintType)
struct FEdgeLoopInfoArrayWrapper
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeLoopInfoArrayWrapper")
    TArray<FEdgeLoopInfo> EdgeLoops;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCapMenuPrompt, FEdgeLoopInfoArrayWrapper, EdgeLoopInfoArray);

struct FEdgeKey
{
    int32 A, B;

    FEdgeKey(int32 InA, int32 InB)
    {
        // Always store the smaller index first to normalize undirected edges
        if (InA < InB)
        {
            A = InA;
            B = InB;
        }
        else
        {
            A = InB;
            B = InA;
        }
    }

    bool operator==(const FEdgeKey& Other) const
    {
        return A == Other.A && B == Other.B;
    }

    friend uint32 GetTypeHash(const FEdgeKey& Edge)
    {
        return HashCombine(GetTypeHash(Edge.A), GetTypeHash(Edge.B));
    }
};

//FORCEINLINE uint32 GetTypeHash(const FVector3f& Vec)
//{
//    return HashCombine(HashCombine(::GetTypeHash(Vec.X), ::GetTypeHash(Vec.Y)), ::GetTypeHash(Vec.Z));
//}
//
//FORCEINLINE uint32 GetTypeHash(const FVector2D& Vec)
//{
//    return HashCombine(::GetTypeHash(Vec.X), ::GetTypeHash(Vec.Y));
//}
//
//FORCEINLINE uint32 GetTypeHash(const FVector& Vec)
//{
//    return HashCombine(HashCombine(::GetTypeHash(Vec.X), ::GetTypeHash(Vec.Y)), ::GetTypeHash(Vec.Z));
//}
//
//struct FVertexKey
//{
//
//
//    FVector3f Position;
//    FVector2D UV;
//    FVector Normal;
//
//    FVertexKey() {}
//    FVertexKey(const FVector3f& InPos, const FVector2D& InUV, const FVector& InNormal)
//        : Position(InPos), UV(InUV), Normal(InNormal) {}
//
//    bool operator==(const FVertexKey& Other) const
//    {
//        constexpr float Tolerance = 0.01f;
//        return Position.Equals(Other.Position, Tolerance)
//            && UV.Equals(Other.UV, Tolerance)
//            && Normal.Equals(Other.Normal, Tolerance);
//    }
//};

//FORCEINLINE uint32 GetTypeHash(const FVertexKey& Key)
//{
//    uint32 Hash = HashCombine(::GetTypeHash(Key.Position), ::GetTypeHash(Key.UV));
//    Hash = HashCombine(Hash, ::GetTypeHash(Key.Normal));
//    return Hash;
//}

UCLASS(Blueprintable, BlueprintType)
class ULimbExtractorTool : public UObject
{
    GENERATED_BODY()

public:
 
#pragma region Functions

    ///////// Called by the editor utility and lets us run either the static mesh or skeletal mesh creation
    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void ExtractBoneToMesh(USkeletalMesh* SkeletalMesh, FName BoneName, bool CreateSkelMesh, bool CreateBoneChain);

    int32 InferLoopMaterialFromPositions(const TArray<FVector3f>& LoopPositions, const TArray<int32>& FinalTriangles, const TArray<FVector3f>& FinalVertices, const TArray<int32>& MaterialIndices, float PositionTolerance);

    // when the editor utility is closed, it clears the static mesh, skeletal mesh and actor references. Deletes actor from the world
    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void OnEditorUtilityClosed();
    
    // spawns our preview actors with respective meshes attached
    UFUNCTION()
    void SpawnProcMeshActor();
    UFUNCTION()
    void  SpawnSkelMeshActor();

    UFUNCTION()
    void SpawnRevCapActor(int32 index);

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void CreateReverseCapPreview(int32 index);

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void MergeCapsWithMesh(bool StaticMesh = false, bool WeldMeshes = true);

    //  Takes our Bone name input and extractc all the weighted vertices associated with it - used during static mesh creation
    void ExtractBoneVertices(USkeletalMesh* SkeletalMesh, FName BoneName, TArray<FVector3f>& OutVertices, TArray<int32>& OutVertexIndices, uint8 MinimumWeightThreshold /*= 128*/, TSet<int32>& SelectedVerts);
    
    //   Takes our Bone name input and extractc all the weighted vertices associated with it - used during skeletal mesh creation  
    void ExtractSkelMeshVertices(USkeletalMesh* SkeletalMesh, FName BoneName, TArray<FRawVertexData>& VertexData, float MinimumWeightThreshold, const TArray<int32>& ChainBoneIndices, TSet<int32> ProcessedVertices);
    
    void GetBoneHierachy(USkeletalMesh* SkeletalMesh, FName RootBone, const TSet<FName>& ExcludedBones, TArray<int32>& OutSortedBoneIndices);

    void CreateSkeletalLimbData(USkeletalMesh* SkeletalMesh, USkeletalMesh*& OutLimbMesh, const TArray<int32>& SortedBoneIndices, TMap<int32, int32>& OutBoneRemap, const TSet<int32>& SkinWeightBoneIndicesUsed);

    FTransform GetAccumulatedTransform(const FReferenceSkeleton& RefSkeleton, int32 BoneIndex);

    void CreateLimbMeshFromRawVertexData(USkeletalMesh* SourceMesh, USkeletalMesh*& OutLimbMesh, const TArray<FVector3f>& Vertices, const TArray<FVector2D>& UVs, const TArray<FVector>& Normals, const TArray<FColor>& Colors, const TArray<FVector>& Tangents, const TArray<FIntVector>& Triangles,  TArray<FRawVertexData> VertexData, const TMap<int32, int32>& BoneRemap, const TSet<int32>& SkinWeightBoneIndicesUsed, TArray<int32> MaterialIndices);

    void ClearCachedDataForRebuild();

    void CreateSkeletalMesh(USkeletalMesh* SkeletalMesh, FName BoneName, bool CreateBoneChain);

    void CreateStaticMesh(USkeletalMesh* SkeletalMesh, FName BoneName, bool CreateBoneChain);

    void BuildEdgeLoopInfoCache(USkeletalMesh* SkeletalMesh);

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void CreateFadeRingAndCap(FEdgeLoopInfo LoopInfo, FEdgeLoopInfo& UpdatedInfo, FCapSkinningInfo Info);

    void CreateReverseCap(FEdgeLoopInfo LoopInfo);

    void BuildBuffersFromRawVertexData(USkeletalMesh* SourceMesh,
        const TArray<FRawVertexData>& VertexData,
        TArray<FVector3f>& OutVertices,
        TArray<FVector>& OutNormals,
        TArray<FVector2D>& OutUVs,
        TArray<FColor>& OutColors,
        TArray<FVector>& OutTangents,
        TArray<int32>& OutOriginalVertexIndices,
        TArray<FSkinWeightInfo>& OutSkinWeights,
        TArray<FIntVector>& OutTriangles, TArray<int32>& OutMaterialIndicesPerTriangle,
        int32 LODIndex);

    void BuildStaticMeshData(USkeletalMesh* SkeletalMesh, const TArray<int32>& ExtractedVertexIndices, TArray<FVector3f>& OutVertices, TArray<FVector2D>& OutUVs, TArray<FVector>& OutNormals, TArray<FColor>& OutColors, TArray<FVector>& OutTangents, TMap<int32, int32>& OutOldToNewVertexMap, TArray<FIntVector>& OutTriangles, TArray<int32>& OutMaterialIndices);

    void DebugTriangleExtraction(USkeletalMesh* SkeletalMesh, const TArray<int32>& ExtractedVertexIndices, const TArray<FIntVector>& OutTriangles, const TArray<int32>& OutMaterialIndices);

    ///////////// The main functions for extracting and processing vertice, UV and Normals data ///////////
        void BuildExtractedMeshData(
            USkeletalMesh* SkeletalMesh,
            const TArray<int32>& BoneVertexIndices,
            TArray<FVector3f>& OutVertices,
            TArray<FVector2D>& OutUVs,
            TArray<FVector>& OutNormals,
            TArray<FColor>& OutColors,
            TArray<FVector>& OutTangents,
            TMap<int32, int32>& OutOldToNewVertexMap,
            TArray<FIntVector>& OutTriangles,
            TArray<FSkinWeightInfo>& OutSkinWeights,
            TArray<int32>& OriginalVertexIndices);

    void BuildTriangleIndices(
        USkeletalMesh* SkeletalMesh,
        const TSet<int32>& BoneVertexIndexSet,
        const TMap<int32, int32>& OldToNewVertexMap,
        TArray<int32>& OutFinalTriangles, TArray<int32>& OutMaterialIndices);

    

    TSet<TPair<int32, int32>> FindOpenEdgesFromTriangles(const TArray<int32> FinTriangles);

    TArray<TArray<int32>> SortOpenEdgesIntoLoops(const TSet<TPair<int32, int32>>& OpenEdges, bool bEnforceCCW);
    
    void AddMaterialSectionToProceduralMesh(
        UProceduralMeshComponent* NewMesh,
        int32 SectionIndex,
        int32 MaterialIndex,
        const TArray<int32>& SectionTriangles,
        const TArray<FVector>& FinalVertices,
        const TArray<FVector>& FinalNormals,
        const TArray<FVector2D>& FinalUVs,
        const TArray<FLinearColor>& FinalColors,
        const TArray<FProcMeshTangent>& MeshTangents,
        const TArray<FSkeletalMaterial>& SkeletalMaterials, bool bSkipRemap);

    void GenerateMaterialToTrianglesMap(const TArray<int32>& MaterialIndices, const TArray<int32>& FinalTriangles, TMap<int32, TArray<int32>>& OutMaterialToTriangles);

    void GenerateCapFanFromLoop(
        const TArray<FVector3f>& Loop,
        const TArray<FVector3f>& FinalVertices,
        TArray<FVector3f>& CapVertices,
        TArray<int32>& CapTriangles,
        TArray<FVector2D>& CapUVs,
        TArray<FVector>& CapNormals
    );

    bool IsLoopTwistRisk(const TArray<FVector3f>& LoopPositions, float PlanarityThreshold, float MaxNormalDeviation, float SegmentSlopeTolerance, int32 SegmentWindow);

    void GenerateDomedCapFromLoop(const TArray<FVector3f>& LoopVerts, TArray<FVector3f>& OutVertices, TArray<int32>& OutTriangles,
        TArray<FVector2D>& OutUVs,
        TArray<FVector>& OutNormals, float DomeHeight, float NoiseAmount, int32 RingCount, bool bApplyNoise);

    void GenerateFadeRingCapFromLoopInfo(const FEdgeLoopInfo& LoopInfo, TArray<FVector3f>& OutVertices, TArray<int32>& OutTriangles, TArray<FVector2D>& OutUVs, TArray<FVector>& OutNormals, TArray<int32>& OutFadeRingIndices, TArray<int32>& OutFadeRingTriangles);

    void GetNormalFromBoneAxis(const FEdgeLoopInfo& LoopInfo, FVector3f& SliceNormal);

    FVector3f ComputeLoopNormalPCA(const TArray<FVector3f>& LoopVerts);

    void GenerateRobustFadeRingCapFromLoopInfo(
        const FEdgeLoopInfo& LoopInfo,
        TArray<FVector3f>& OutVertices,
        TArray<int32>& OutTriangles,
        TArray<FVector2D>& OutUVs,
        TArray<FVector>& OutNormals,
        TArray<FProcMeshTangent>& OutTangents,
        TArray<FLinearColor>& OutVertexColors,
        TArray<int32>& OutFadeRingIndices,
        TArray<int32>& OutFadeRingTriangles
    );

    void GenerateReverseFadeRingFromLoopInfo(const FEdgeLoopInfo& LoopInfo, TArray<FVector3f>& OutVertices, TArray<int32>& OutTriangles, TArray<FVector2D>& OutUVs, TArray<FVector>& OutNormals, TArray<FProcMeshTangent> OutTangents, TArray<FLinearColor> OutVertexColors, TArray<int32>& OutFadeRingIndices, TArray<int32>& OutFadeRingTriangles);
    
        //////// Cap UV generation /////////

    void ProjectPlanarUVsForCap(
        const TArray<FVector3f>& Vertices,
        int32 StartIndex,
        int32 Count,
        const FVector3f& PlaneOrigin,
        const FVector3f& PlaneNormal,
        TArray<FVector2D>& UVs,
        FVector2D UVScale,
        float UVRotationDegrees,
        FVector2D UVOffset
    );

    static FVector3f ComputeLoopNormalWithTolerance(const TArray<FVector3f>& LoopVerts, float ToleranceMultiplier = 1.5f  /* how loose the filter is */ );
    ///  Experimental stuff /////////
    void FindOpenEdgesFromDescription(const FMeshDescription& MeshDescription, TArray<TArray<int32>>& EdgeLoops);
    void FindBoundaryLoopsFromTriangles(const TArray<FIntVector>& Triangles, TArray<TArray<int32>>& OutLoops);

    void GenerateFlowBasedFadeUVs(const TArray<FVector3f>& BaseLoopPositions, const TArray<FVector2D>& BaseUVs, const TArray<int32>& OutFadeRingIndices, TArray<FVector2D>& OutUVs, const TArray<FVector3f>& OutVertices, FVector3f SliceNormal);

    float EstimateWorldToUVScale(const TArray<FVector3f>& WorldPositions, const TArray<FVector2D>& UVs);

    // Logging and Visualizers /////////
    // Called by the editor utility for logging
    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Logging")
    void LogBoneIndices(USkeletalMesh* SkeletalMesh, bool bAlsoLogSkeleton /*= false*/);

    void DrawLoopNormalDebug(const TArray<FVector3f>& LoopVerts, const FVector3f& Normal, FColor Color, float Length, float Duration);

    void PrintError(FString string);

    void  DrawEdgeLoopsDebug(
        const TArray<TArray<FVector3f>>& EdgeLoops,
        const TArray<FVector3f>& Vertices,
        UWorld* World,
        const FTransform& MeshTransform,
        float Duration,
        float Thickness);

    void ValidateVertexToSkinWeightMapping(const USkeletalMesh* SourceMesh, const TArray<FVector3f>& OutVertices, const TArray<FSkinWeightInfo>& OutSkinWeights, const TArray<int32>& OriginalVertexIndices);

   ////////  For checking vertice adjacency when debugging edgeloops
    void LogVertexAdjacency(const TArray<FIntVector>& Triangles, int32 VertexIndex);
    
public:

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void PopulateSkelHierachy(USkeletalMesh* SkeletalMesh);

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void SetMinimumWeightThreshold(int32 value);

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void SetExclusionBones(TArray<FName> Exclusions);
    
    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void ClearExcusionBoneList();

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void LoggingAndDebugSwitches(bool ShowLoops, bool MeshDiagnostic);

    UFUNCTION(BlueprintPure, Category = "Limb Tools|Functions")
    AActor* GetOwnerActor();

  private:

      // New helper functions for edgeloop clustering experiment
      void ProcessEdgeLoopsWithClustering(TArray<FEdgeLoopInfo>& EdgeLoopInfoArray);
      FEdgeLoop ConvertEdgeLoopInfoToEdgeLoop(const FEdgeLoopInfo& LoopInfo);
      FEdgeLoopInfo ConvertEdgeLoopToEdgeLoopInfo(const FEdgeLoop& EdgeLoop, int32 OriginalLoopNumber = -1);
      void UpdateEdgeLoopInfosFromClusters(const TArray<FEdgeLoopCluster>& Clusters);

      FEdgeLoopInfo* FindOriginalEdgeLoopInfo(const FEdgeLoop& EdgeLoop, TArray<FEdgeLoopInfo>& EdgeLoopInfoArray);

      void DebugDrawClusters();


#pragma endregion

private:
    ///// The main use members /////////////////
    UPROPERTY()
    UProceduralMeshComponent* PreviewMesh;

    UPROPERTY()
    AActor* OwnerActor;

    UPROPERTY()
    USkeletalMesh* OutLimbSkelMesh;

    UPROPERTY()
    USkeletalMeshComponent* SkelMesh;

    UPROPERTY()
    FEdgeLoopInfoArrayWrapper Wrapper;

    UPROPERTY()
    USkeletalMesh* InMesh;

    UPROPERTY()
	TArray<FRawVertexData> RawVertexData;
    ///// To broadcast to the menu ////////
    UPROPERTY(BlueprintAssignable, Category = "Limb Tools|Callbacks")
    FCapMenuPrompt Prompt;

protected:
        // for when we are iterating over EdgeLoopInfos 
        int32 LoopPreviewIndex = 0;

        // This is our Cached output from the static mesh creation - used in the capping section
        FMeshBuildContext CachedMeshContext;

        // This is our EdgeLoop Info, so we can iterate over it and decided whether we want to cap it or not
        TArray<FEdgeLoopInfo> EdgeLoopInfos;

		// --- These hold info about the generated caps, and how the user wants to skin them, so we can use and update them later if needed --- 
		// a collection of the generated meshes so we can update them  ** populated along with CapSkinningInfos **
        TMap<int32, UProceduralMeshComponent*> CapComponents;

        // This is to hold our map of reverse caps to match the generated caps - i:e - wounds to attach to the character mesh
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Limb Tools|ReverseCaps")
        TMap<int32, UProceduralMeshComponent*> ReverseCapComponents;

        // a collection of ReverseCap Actors spawned in the world
        TMap<int32, AActor*> RevCapActors;

		// a collection of FCapSkinningInfo structs that hold the skinning options for each cap  ** populated along with CapComponents **
        TMap<int32, FCapSkinningInfo> CapSkinningInfos;
        
public:
    ////////  Exposed members for use in the menu //////////////////
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Variables")
    TArray<FName> ExcludedBoneNames;

    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Variables")
     TArray<FBoneHierachyStruct> BoneHierachy;
     
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Variables")
     uint8 MinimumWeight = 0; // was 128, but in the end we want it variable to enable holed mesh construction, and shorter setions. 0 is so every vert is chosen

     ////////  Set using the public function for debug //////////////////
     UPROPERTY()
     bool ShowLoopDebug = true;

     UPROPERTY()
     bool DoDiagnostic = false;

     /////// Exposed members for the editor tool to control UV layout on the caps  ////////////////////
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Controls")
     TArray<FVector3f> BP_CapVertCache;	//Cap triangle vertex positions(flat)
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Controls")
     FVector3f BP_PlaneOrigin;	//A point on the slicing plane(e.g.loop centroid)
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Controls")
     FVector3f BP_PlaneNormal;	//Normal of the slicing plane(facing out from limb)
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Controls")
     TArray<FVector2D> BP_OutUVs;   //Output UVs(1:1 with CapVertices)
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Controls")
     float	BP_UVScale = 0.01f;         //Controls tiling(default 1.0f)
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Controls")
     float	BP_UVRotationDegrees = 0.0f;    //Degrees to rotate UVs around center
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Controls")
     FVector2D BP_UVOffset = FVector2D(0.f, 0.f);    //UV translation(useful for fitting different caps)
     
     ////// Reverse Cap FadeRing UV controls
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Controls")
     float	BP_RCFRUVScale = 0.01f;         //Controls tiling(default 1.0f)
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Controls")
     float	BP_RCFRUVRotationDegrees = 0.0f;    //Degrees to rotate UVs around center
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Controls")
     FVector2D BP_RCFRUVOffset = FVector2D(0.f, 0.f);    //UV translation(useful for fitting different caps)
    
     ///////////////// Exposed members to control cap doming, material, and noise /////////////////////
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Texture")
     TSoftObjectPtr<UMaterialInterface> BP_CapMaterial;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     float BP_DomeHieght = -2.0f;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     float BP_FadeRingLength = -2.0f;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     float BP_NoiseAmount = 0.1;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     int32 BP_RingCount = 1;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     bool BP_AddNoise = false;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     bool AddDome = false;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     bool AddCap = true;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     float EffectiveThreshold = 0.001f; // smaller is better

	 /////////////////////////////////////////////////////////////////////////////////////////////////////
      // New clustering configuration properties
     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edge Loop Clustering")
     bool bEnableLoopClustering = true;

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edge Loop Clustering", meta = (EditCondition = "bEnableLoopClustering"))
     float ClusterRadius = 5.0f;

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edge Loop Clustering", meta = (EditCondition = "bEnableLoopClustering"))
     float MinLoopArea = 0.5f;

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edge Loop Clustering", meta = (EditCondition = "bEnableLoopClustering"))
     bool bMergeOverlappingLoops = false; // DISABLED BY DEFAULT to preserve data

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edge Loop Clustering", meta = (EditCondition = "bEnableLoopClustering"))
     bool bFilterSmallFragments = true;

     // New safety parameters
     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edge Loop Clustering", meta = (EditCondition = "bEnableLoopClustering"))
     float MaxMergeDistance = 1.0f; // Only merge loops within this distance

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edge Loop Clustering", meta = (EditCondition = "bEnableLoopClustering"))
     bool bPreserveAllVertexIndices = true; // Safety flag to never lose VertexIndices

     // Enhanced data structures
     UPROPERTY(BlueprintReadOnly, Category = "Edge Loop Results")
     TArray<FEdgeLoopCluster> DetectedClusters;

     UPROPERTY(BlueprintReadOnly, Category = "Edge Loop Results")
     TArray<FEdgeLoopInfo> ClusteredEdgeLoopInfos;


};

