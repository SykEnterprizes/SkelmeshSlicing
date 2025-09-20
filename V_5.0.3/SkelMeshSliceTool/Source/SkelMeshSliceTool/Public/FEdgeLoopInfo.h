#pragma once

#include "CoreMinimal.h"

//#include </Program Files/Epic Games/UE_5.0/Engine/Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h>

#include "FEdgeLoopInfo.Generated.h"

USTRUCT(BlueprintType)
struct FEdgeLoopInfo
{
	GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    TArray<int32> VertexIndices;  // vertiex indices are the indices from the output sorted edgeloop array
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    TArray<FVector3f> Positions;  // the actual vectors of the edgeloop vertices
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    TArray<FVector3f> FadeRingPositions;  // the actual vectors of the generated fadering vertices
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    TArray<FVector2D> UVs;   // the uvs assigned to the edgeloops
   
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    int32 VotedMaterialIndex;  // the material index designated from the base mesh
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float TotalEdgeLength;  // unused yet.. but maybe good for something later
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    bool bAddFadeRing = false;  // add a fade ring to help with excessively uneven edgeloops
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    bool bAddDome = false;  // add a dome shape, or just a fan cap
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    bool bAddNoise = false;  // toggles adding noise to the procedural generation
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    bool bFlatten = false;  // toggles flat slicing the cap generation
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    bool bUseManualNormal = false;
    
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    int32 LoopNumber = 0;  // an identifier for the loop  **** This shouldnt be changed since its an identifier, and if we break a struct in bp, unless its filled exactly, will cause issues with other loops
 	UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
	float DomeHeight = -2.0f;  // height of the dome, if applicable
	UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
	float FadeRingHieght = -2.0f;  // hieght of the fadering, if applicable
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    int32 RingCount = 2;  // number of inner rings to add to the fade ring
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float NoiseAmount = 0.1f;  // amount of noise to apply to the cap
    
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float UV_Rotation = 0.0f;  // amount of rotation we can add to the UV assignment of the cap or dome
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float UV_Scale = 1.0f; // amount of scale we can ad to the UV assignment of the cap or dome
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float UV_Offset_X = 0.0f; // amount of X offset we can assign to the UV in the cap or dome
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float UV_Offset_Y = 0.0f; // amount of Y offset we can assign the UV in the cap or dome
    
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    bool TwistFixOverride = false; // override for the loop twist-risk checker
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float PlanitaryThreshold = 1.0f; // Max allowed vertical offset between loop points from a best-fit plane.  High = tolerant of warped loops.  Low = flags even mild bends 
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float MaxNormalDeviation = 0.8f; // Allowed angle (in degrees) between segment normals and expected orientation.  Controls how much normal flipping is considered “twisted.” Great for detecting shading artifacts.
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float SegmentSlopeTolerance = 0.3f;  //  Acceptable slope variance between loop segments.  Helps catch loops with uneven contour, which may hint at visual twisting.
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    int32 SegmentWindow = 3;  //  Number of segments to include in each local comparison window.  Smaller = detects tight twists. Larger = favors broader averaging.
    
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    TSoftObjectPtr<UMaterialInterface> CapMaterial;  // material to apply to the cap
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    TSoftObjectPtr<UMaterialInterface> FadeRingMaterial;  // material to apply to the fadering ** optional and tbd since we already get it from InferMaterialFromPositions()** 
    
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    FText AssociatedBone;  // give the loop an associated bone to do normal direction calc
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float LimbEndCollapseAmount = 0.05;  // the amount the fadering will collapse by
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float ReverseCapFadeAmount = 5.0f;
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float RevCapExpansionAmount = 0.05;
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    int32 RevCapFadeRingCount = 2;  // number of inner rings to add to the fade ring
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    int32 FadeRingCount = 2;  // number of inner rings to add to the fade ring
    
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float UV_RCFRScale = 1.0f; // amount of scale we can ad to the UV assignment of the cap or dome
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float UV_RCFROffset_X = 0.0f; // amount of X offset we can assign to the UV in the cap or dome
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float UV_RCFROffset_Y = 0.0f; // amount of Y offset we can assign the UV in the cap or dome
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|EdgeloopInfo")
    float UV_RCFRRotation = 0.0f;  // amount of rotation we can add to the UV assignment of the cap or dome
};

