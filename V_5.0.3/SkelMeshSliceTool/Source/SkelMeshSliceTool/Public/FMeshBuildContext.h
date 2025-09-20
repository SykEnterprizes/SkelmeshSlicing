#pragma once
 
#include "CoreMinimal.h"
#include "FMeshBuildContext.Generated.h"

    USTRUCT(BlueprintType)
    struct FMeshBuildContext
{
    GENERATED_BODY()

public:

    TArray<FVector3f> FinalVertices;
    TArray<int32> FinalTriangles;
    TArray<FVector2D> FinalUVs;
    TArray<FVector> FinalNormals;
    TArray<FVector> FinalTangents;
    TArray<int32> FinalMaterialIndices;
    TArray<FIntVector> IntVecTriangles;
    TArray<FSkeletalMaterial> MeshMaterials; // i added this so we can store the materials, mainly for the use in a static mesh build function, because we can get the skeletal mesh materials easy enough

    void Clear()
    {
        FinalVertices.Reset();
        FinalTriangles.Reset();
        FinalUVs.Reset();
        FinalNormals.Reset();
        FinalTangents.Reset();
        FinalMaterialIndices.Reset();
        IntVecTriangles.Reset();
        MeshMaterials.Reset();
    }

};



