#pragma once

#include "CoreMinimal.h"

#include </Program Files/Epic Games/UE_5.0/Engine/Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h>

#include "FEdgeLoopInfo.Generated.h"

USTRUCT(BlueprintType)
struct FEdgeLoopInfo
{
	GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite)
    TArray<int32> VertexIndices;  // vertiex indices are the indices from the output sorted edgeloop array
    UPROPERTY(BlueprintReadWrite)
    TArray<FVector3f> Positions;  // the actual vectors of the vertices
    UPROPERTY(BlueprintReadWrite)
    TArray<FVector2D> UVs;
    UPROPERTY(BlueprintReadWrite)
    int32 VotedMaterialIndex;  // the material index designated from the base mesh
    UPROPERTY(BlueprintReadWrite)
    float TotalEdgeLength;  // unused yet.. but maybe good for something later
    UPROPERTY(BlueprintReadWrite)
    bool bAddFadeRing = false;  // add a fade ring to help with excessively uneven edgeloops
    UPROPERTY(BlueprintReadWrite)
    bool bAddDome = false;  // add a dome shape, or just a fan cap
    UPROPERTY(BlueprintReadWrite)
    bool bAddNoise = false;  // toggles adding noise to the procedural generation
    UPROPERTY(BlueprintReadWrite) //  This shouldnt be changed since its an identifier, and if we break a struct in bp, unless its filled exactly, will cause issues with other loops
    int32 LoopNumber = 0;  // an identifier for the loop
	UPROPERTY(BlueprintReadWrite)
	float DomeHeight = -2.0f;  // height of the dome, if applicable
	UPROPERTY(BlueprintReadWrite)
	float FadeRingHieght = -2.0f;  // hieght of the fadering, if applicable
    UPROPERTY(BlueprintReadWrite)
    int32 RingCount = 2;  // number of rings to add to the fade ring
    UPROPERTY(BlueprintReadWrite)
    float NoiseAmount = 0.1f;  // amount of noise to apply to the cap
    UPROPERTY(BlueprintReadWrite)
    TSoftObjectPtr<UMaterialInterface> CapMaterial;  // material to apply to the cap
    UPROPERTY(BlueprintReadWrite)
    TSoftObjectPtr<UMaterialInterface> FadeRingMaterial;  // material to apply to the fadering ** optional and tbd since we already get it from InferMaterialFromPositions()** 
    
};

