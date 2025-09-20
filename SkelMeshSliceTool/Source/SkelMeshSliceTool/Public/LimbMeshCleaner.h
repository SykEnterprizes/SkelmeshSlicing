#pragma once

#include "CoreMinimal.h"


class UMeshCleanUtils : public UObject 
{

public:
    // Master cleanup entry
   static void UtilCleanMeshForCapping(
        TArray<FVector3f>& Vertices,
        TArray<FIntVector>& Triangles, TArray < FVector2D>& InOutUVs, TArray<FVector>& InOutNormals,
        float WeldThreshold = 0.01f,
        int32 ValenceThreshold = 10, bool RunDiagnostic = false);

    static void WeldVertices(
        TArray<FVector3f>& Vertices,
        TArray<FIntVector>& Triangles,
        TArray<FVector2D>& InOutUVs,
        TArray<FVector>& InOutNormals,
        float WeldThreshold);

    static void RemoveDegenerateTriangles(
        TArray<FIntVector>& Triangles);

    static void RemoveDuplicateTriangles(TArray<FIntVector>& Triangles);

    static void RemoveNonManifoldTriangles(
        TArray<FIntVector>& Triangles);

    static void RemoveLooseBridges(TArray<FVector3f>& Vertices, TArray<FIntVector>& Triangles, float MaxBridgeLength = 50.f);

    static void DiagnoseMeshIntegrity(
        const TArray<FVector3f>& Vertices,
        const TArray<FIntVector>& Triangles,
        int32 ValenceThreshold);
};
