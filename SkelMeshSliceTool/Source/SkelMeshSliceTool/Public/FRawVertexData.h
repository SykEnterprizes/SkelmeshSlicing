#pragma once

#include "CoreMinimal.h"
#include "FRawVertexData.Generated.h"

USTRUCT(BlueprintType)

struct FRawVertexData
{
    GENERATED_BODY()

public:

    FVector3f Position; // Vertex position
    TArray<int32> BoneInfluences;  // Vertex Bone influences
    TArray<float> BoneWeights;  // Vertex skinweight value
    int32 OriginalVertexIndex; // The vertex indice from the skinweight cahe
    uint8 CompactBoneIndex;  // the affected vertex corresponding compact bone index
    int32 RemappedIndex;  // a running tally of vertex remapped from 0 - to total extracted mesh verts
    TArray<FName> BoneNames;        // Store bone names instead of indices
};

