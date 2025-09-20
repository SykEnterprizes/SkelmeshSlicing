#pragma once

#include "CoreMinimal.h"
#include "ESkinningType.h"
#include "FCapSkinningInfo.generated.h"

USTRUCT(BlueprintType)

struct FCapSkinningInfo
{
	GENERATED_BODY()

public:
	
	int32 StartVertexIndex;  // set in code during the Mesh building 
	int32 EndVertexIndex;  // set in code during the Mesh bilding
	UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|CapSkinningInfo")
	ESkinningType SkinningMethod;  // choose between Rigid binding and Proximity based binding
	UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|CapSkinningInfo")
	float ProximityRadius = 10.0f; // Custom radius for this cap if using proximity

};

