#pragma once

#include "CoreMinimal.h"
#include "BoneHierachyStruct.Generated.h"

USTRUCT(BlueprintType)
struct FBoneHierachyStruct
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite)
    FName Bone;
    UPROPERTY(BlueprintReadWrite)
    FName ParentBone;
    UPROPERTY(BlueprintReadWrite)
    TArray<FName> ChildBones;
    UPROPERTY(BlueprintReadWrite)
    int32 BoneIndex;
    UPROPERTY(BlueprintReadWrite)
    FTransform BoneTransform;
    UPROPERTY(BlueprintReadWrite)
    bool BoneVisibility;
    UPROPERTY(BlueprintReadWrite)
    bool IsExpanded;
};

