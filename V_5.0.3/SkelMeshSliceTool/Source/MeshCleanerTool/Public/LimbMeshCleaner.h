#pragma once

#include "CoreMinimal.h"


class MeshCleanUtils
{
public:
    // Master cleanup entry
    static void CleanMeshForCapping(
        TArray<FVector3f>& Vertices,
        TArray<FIntVector>& Triangles,
        float WeldThreshold = 0.01f,
        int32 ValenceThreshold = 10);

private:
    static void WeldVertices(
        TArray<FVector3f>& Vertices,
        TArray<FIntVector>& Triangles,
        float Threshold);

    static void RemoveDegenerateTriangles(
        TArray<FVector3f>& Vertices,
        TArray<FIntVector>& Triangles);

    static void RemoveNonManifoldTriangles(
        TArray<FVector3f>& Vertices,
        TArray<FIntVector>& Triangles);

    static void RemoveLooseBridges(
        TArray<FVector3f>& Vertices,
        TArray<FIntVector>& Triangles,
        float MaxBridgeLength = 50.f);

    static void DiagnoseMeshIntegrity(
        const TArray<FVector3f>& Vertices,
        const TArray<FIntVector>& Triangles,
        int32 ValenceThreshold);
};
