#pragma once
#include "CoreMinimal.h"
#include </Program Files/Epic Games/UE_5.0/Engine/Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h>
#include "BoneHierachyStruct.h"
#include "MeshDescription.h"
#include "StaticMeshAttributes.h"
#include "MeshDescriptionBuilder.h"
#include "LimbMeshCleaner.h"
#include "ULimbExtractorTool.generated.h"


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

FORCEINLINE uint32 GetTypeHash(const FVector3f& Vec)
{
    return HashCombine(HashCombine(::GetTypeHash(Vec.X), ::GetTypeHash(Vec.Y)), ::GetTypeHash(Vec.Z));
}

FORCEINLINE uint32 GetTypeHash(const FVector2D& Vec)
{
    return HashCombine(::GetTypeHash(Vec.X), ::GetTypeHash(Vec.Y));
}

FORCEINLINE uint32 GetTypeHash(const FVector& Vec)
{
    return HashCombine(HashCombine(::GetTypeHash(Vec.X), ::GetTypeHash(Vec.Y)), ::GetTypeHash(Vec.Z));
}

struct FVertexKey
{


    FVector3f Position;
    FVector2D UV;
    FVector Normal;

    FVertexKey() {}
    FVertexKey(const FVector3f& InPos, const FVector2D& InUV, const FVector& InNormal)
        : Position(InPos), UV(InUV), Normal(InNormal) {}

    bool operator==(const FVertexKey& Other) const
    {
        constexpr float Tolerance = 0.01f;
        return Position.Equals(Other.Position, Tolerance)
            && UV.Equals(Other.UV, Tolerance)
            && Normal.Equals(Other.Normal, Tolerance);
    }
};

FORCEINLINE uint32 GetTypeHash(const FVertexKey& Key)
{
    uint32 Hash = HashCombine(::GetTypeHash(Key.Position), ::GetTypeHash(Key.UV));
    Hash = HashCombine(Hash, ::GetTypeHash(Key.Normal));
    return Hash;
}

UCLASS(Blueprintable, BlueprintType)
class ULimbExtractorTool : public UObject
{
    GENERATED_BODY()

public:
 
#pragma region Functions

    ///////// Called by the editor utility
    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void ExtractBoneToStaticMesh(USkeletalMesh* SkeletalMesh, FName BoneName);

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void OnEditorUtilityClosed();
    ////////// Since we are spawning something, ufunction is good for gc
    UFUNCTION()
    void SpawnProcMeshActor();
    void  SpawnSkelMeshActor();
    ///////// Called by the editor utility
    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Logging")
    void LogBoneIndices(USkeletalMesh* SkeletalMesh, bool bAlsoLogSkeleton /*= false*/);

    // Optional TO-DO: export whole bone chain or just 1 bone
    // 
    /////////  This is the main called fucntion, which class all the other listed functions
    void ExtractBoneVertices(USkeletalMesh* SkeletalMesh, FName BoneName, TArray<FVector3f>& OutVertices, TArray<int32>& OutVertexIndices, uint8 MinimumWeightThreshold /*= 128*/, TSet<int32>& SelectedVerts);

    void GetBoneHierachy(USkeletalMesh* SkeletalMesh,
        FName RootBone,
        const TSet<FName>& ExcludedBones,
        TArray<int32>& OutSortedBoneIndices);

    void CreateSkeletalLimbData(USkeletalMesh* SkeletalMesh,
        USkeletalMesh*& OutLimbMesh,
        const TArray<int32>& SortedBoneIndices,
        TMap<int32, int32>& OutBoneRemap, const TSet<int32>& SkinWeightBoneIndicesUsed);

    FTransform GetAccumulatedTransform(const FReferenceSkeleton& RefSkeleton, int32 BoneIndex);

    void CreateLimbMeshFromExtractedData(
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
        TArray<int32>& OriginalVertexIndices
        );

    void ValidateVertexToSkinWeightMapping(const USkeletalMesh* SourceMesh, const TArray<FVector3f>& OutVertices, const TArray<FSkinWeightInfo>& OutSkinWeights, const TArray<int32>& OriginalVertexIndices);

    void ValidateInfluenceBoneMapping(USkeletalMesh* SourceMesh, const TMap<int32, int32>& BoneRemap, const TArray<FSkinWeightInfo>& SkinWeights);

    void ExpandBoneRemapToMatchSkinWeights(const USkeletalMesh* SourceMesh, TMap<int32, int32>& BoneRemap, const TArray<FSkinWeightInfo>& SkinWeights);

    void TopologicallySortBones(const FReferenceSkeleton& RefSkeleton, const TSet<int32>& BoneIndicesIn, TArray<int32>& OutSorted);

    void CreateSkeletalMesh(USkeletalMesh* Mesh, FName Bone);

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

    void GenerateCapFanFromLoop(
        const TArray<int32>& Loop,
        const TArray<FVector3f>& FinalVertices,
        TArray<FVector3f>& CapVertices,
        TArray<int32>& CapTriangles);


    void FindBoundaryLoopsFromTriangles(const TArray<FIntVector>& Triangles, TArray<TArray<int32>>& OutLoops);

    void GenerateDomedCapFromLoop(const TArray<FVector3f>& LoopVerts, TArray<FVector3f>& OutVertices, TArray<int32>& OutTriangles,
        TArray<FVector2D>& OutUVs,
        TArray<FVector>& OutNormals, float DomeHeight, float NoiseAmount, int32 RingCount, bool bApplyNoise);

   
    //////// Cap UV generation /////////

    void ProjectPlanarUVsForCap(const TArray<FVector3f>& CapVertices, const FVector3f& PlaneOrigin, const FVector3f& PlaneNormal, TArray<FVector2D>& OutUVs, float UVScale, float UVRotationDegrees, FVector2D UVOffset);

    static FVector3f ComputeLoopNormalWithTolerance(const TArray<FVector3f>& LoopVerts, float ToleranceMultiplier = 1.5f  /* how loose the filter is */ );
    ///  Experimental stuff /////////
    void FindOpenEdgesFromDescription(const FMeshDescription& MeshDescription, TArray<TArray<int32>>& EdgeLoops);

    void ConvertProceduralMeshToMeshDescription(UProceduralMeshComponent* ProcMeshComp, FMeshDescription& MeshDescription);

    // Logging and Visualizers /////////
    void PrintError(FString string);

    void  DrawEdgeLoopsDebug(
        const TArray<TArray<int32>>& EdgeLoops,
        const TArray<FVector3f>& Vertices,
        UWorld* World,
        const FTransform& MeshTransform,
        float Duration,
        float Thickness);


    void SpawnTextLabelInEditor(UWorld* World, const FVector& Position, const FString& Text, const FColor& Color);

    void LogVertexAdjacency(const TArray<FIntVector>& Triangles, int32 VertexIndex);





public:

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void PopulateSkelHierachy(USkeletalMesh* SkeletalMesh);

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void SetMinimumWeightThreshold(int32 value);

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void AddExclusionBones(TArray<FName> Exclusions);

    UFUNCTION(BlueprintCallable, Category = "Limb Tools|Functions")
    void LoggingAndDebugSwitches(bool ShowLoops, bool MeshDiagnostic);
  

#pragma endregion

private:
    
    UPROPERTY()
    UProceduralMeshComponent* PreviewMesh;

    UPROPERTY()
    AActor* OwnerActor;

    UPROPERTY()
    USkeletalMesh* OutLimbSkelMesh;

public:

    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Variables")
    TArray<FName> ExcludedBoneNames;

    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Variables")
     TArray<FBoneHierachyStruct> BoneHierachy;
     
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Variables")
     uint8 MinimumWeight = 128;

     UPROPERTY()
     bool ShowLoopDebug = true;

     UPROPERTY()
     bool DoDiagnostic = false;

     /////// Exposed members for the editor tool to control UV layout on the caps
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
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|UV Texture")
     TSoftObjectPtr<UMaterialInterface> BP_CapMaterial;


     ///////////////// Exposed members to control cap doming and noise /////////////////////
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     float BP_DomeHieght = 5.0f;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     float BP_NoiseAmount = 0.1;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     int32 BP_RingCount = 1;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     bool BP_AddNoise = false;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     bool AddDome = false;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     bool AddCap = false;
     UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|Cap structure controls")
     bool CreateSkelMesh = true;

    
};
