#pragma once

#include "CoreMinimal.h"
#include "BoneHierachyStruct.Generated.h"

USTRUCT(BlueprintType)
struct FBoneHierachyStruct
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|BoneHierachy")
    FName Bone;
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|BoneHierachy")
    FName ParentBone;
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|BoneHierachy")
    TArray<FName> ChildBones;
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|BoneHierachy")
    int32 BoneIndex;
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|BoneHierachy")
    FTransform BoneTransform;
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|BoneHierachy")
    bool BoneVisibility;
    UPROPERTY(BlueprintReadWrite, Category = "Limb Tools|BoneHierachy")
    bool IsExpanded;
};

