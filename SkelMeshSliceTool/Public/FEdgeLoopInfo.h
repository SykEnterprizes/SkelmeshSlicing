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
    TArray<int32> VertexIndices;
    UPROPERTY(BlueprintReadWrite)
    TArray<FVector3f> Positions;
    UPROPERTY(BlueprintReadWrite)
    int32 VotedMaterialIndex;
    UPROPERTY(BlueprintReadWrite)
    float TotalEdgeLength;
    UPROPERTY(BlueprintReadWrite)
    bool bShouldCap = false; // toggleable
    UPROPERTY(BlueprintReadWrite)
    bool bAddFadeRing = false;
    UPROPERTY(BlueprintReadWrite)
    bool bAddDome = false;
    UPROPERTY(BlueprintReadWrite)
    bool bAddNoise = false;
    UPROPERTY(BlueprintReadWrite)
    int32 LoopNumber = 0;
    // didnt want this exposed, but since we make struct data in the cap widget
    // we need it exposed to make sure we can pass the value to the new struct data
    UPROPERTY(BlueprintReadWrite)
    UProceduralMeshComponent* GeneratedCap;
};

