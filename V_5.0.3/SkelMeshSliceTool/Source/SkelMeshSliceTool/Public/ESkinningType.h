#pragma once
#include "CoreMinimal.h"
#include "ESkinningType.generated.h"

UENUM(BlueprintType)
enum class ESkinningType : uint8
{
    ProximityBlending UMETA(DisplayName = "Proximity Blending", Category = "Limb Tools|CapSkinningType"),
    RigidClosestBone UMETA(DisplayName = "Rigid Closest Bone", Category = "Limb Tools|CapSkinningType")
};

