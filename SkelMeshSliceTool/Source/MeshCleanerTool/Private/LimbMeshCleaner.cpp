
// LimbMeshCleaner.cpp
#include "LimbMeshCleaner.h"
#include "Containers/Set.h"
#include "Logging/LogMacros.h"

void ULimbCleanerTool::CleanMeshForCapping(TArray<FVector3f>& Vertices, TArray<FIntVector>& Triangles)
{
    WeldVertices(Vertices, Triangles, 0.01f);
    RemoveDegenerateTriangles(Triangles);
    RemoveDuplicateTriangles(Triangles);
    RemoveNonManifoldTriangles(Triangles);
    DiagnoseMeshIntegrity(Vertices, Triangles);
}

void ULimbExtractorTool::WeldVertices(TArray<FVector3f>& Vertices, TArray<FIntVector>& Triangles, float WeldThreshold)
{
    TArray<int32> Remap;
    Remap.SetNum(Vertices.Num());
    TArray<FVector3f> UniqueVerts;
    UniqueVerts.Reserve(Vertices.Num());

    for (int32 i = 0; i < Vertices.Num(); ++i)
    {
        const FVector3f& V = Vertices[i];
        int32 FoundIndex = INDEX_NONE;
        for (int32 j = 0; j < UniqueVerts.Num(); ++j)
        {
            if (FVector3f::DistSquared(UniqueVerts[j], V) <= WeldThreshold * WeldThreshold)
            {
                FoundIndex = j;
                break;
            }
        }

        if (FoundIndex != INDEX_NONE)
        {
            Remap[i] = FoundIndex;
        }
        else
        {
            Remap[i] = UniqueVerts.Add(V);
        }
    }

    Vertices = UniqueVerts;

    for (FIntVector& Tri : Triangles)
    {
        Tri.X = Remap[Tri.X];
        Tri.Y = Remap[Tri.Y];
        Tri.Z = Remap[Tri.Z];
    }
}

void ULimbCleanerTool::RemoveDegenerateTriangles(TArray<FIntVector>& Triangles)
{
    Triangles.RemoveAll([](const FIntVector& Tri)
    {
        return Tri.X == Tri.Y || Tri.Y == Tri.Z || Tri.Z == Tri.X;
    });
}

void ULimbCleanerTool::RemoveDuplicateTriangles(TArray<FIntVector>& Triangles)
{
    TSet<FIntVector> UniqueTris;
    Triangles.RemoveAll([&UniqueTris](const FIntVector& Tri)
    {
        FIntVector SortedTri = Tri;
        int32* Data = &SortedTri.X;
        Algo::Sort(Data, 3);
        return !UniqueTris.Add(SortedTri);
    });
}

void ULimbCleanerTool::RemoveNonManifoldTriangles(TArray<FIntVector>& Triangles)
{
    TMap<TPair<int32, int32>, int32> EdgeCount;
    for (const FIntVector& Tri : Triangles)
    {
        int32 Indices[3] = { Tri.X, Tri.Y, Tri.Z };
        for (int32 i = 0; i < 3; ++i)
        {
            int32 A = Indices[i];
            int32 B = Indices[(i + 1) % 3];
            if (A > B) Swap(A, B);
            TPair<int32, int32> Edge(A, B);
            EdgeCount.FindOrAdd(Edge)++;
        }
    }

    Triangles.RemoveAll([&EdgeCount](const FIntVector& Tri)
    {
        int32 Indices[3] = { Tri.X, Tri.Y, Tri.Z };
        for (int32 i = 0; i < 3; ++i)
        {
            int32 A = Indices[i];
            int32 B = Indices[(i + 1) % 3];
            if (A > B) Swap(A, B);
            TPair<int32, int32> Edge(A, B);
            if (EdgeCount[Edge] > 2) return true;
        }
        return false;
    });
}

void ULimbCleanerTool::DiagnoseMeshIntegrity(const TArray<FVector3f>& Vertices, const TArray<FIntVector>& Triangles)
{
    TMap<TPair<int32, int32>, int32> EdgeUseCount;
    TMap<int32, int32> VertexEdgeUseCount;
    int32 DegenerateTriangleCount = 0;

    for (const FIntVector& Tri : Triangles)
    {
        int32 Indices[3] = { Tri.X, Tri.Y, Tri.Z };

        if (Tri.X == Tri.Y || Tri.Y == Tri.Z || Tri.Z == Tri.X)
        {
            UE_LOG(LogTemp, Warning, TEXT("Degenerate triangle: (%d, %d, %d)"), Tri.X, Tri.Y, Tri.Z);
            DegenerateTriangleCount++;
        }

        for (int i = 0; i < 3; ++i)
        {
            int32 A = Indices[i];
            int32 B = Indices[(i + 1) % 3];
            if (A > B) Swap(A, B);
            TPair<int32, int32> Edge(A, B);
            EdgeUseCount.FindOrAdd(Edge) += 1;
            VertexEdgeUseCount.FindOrAdd(A) += 1;
            VertexEdgeUseCount.FindOrAdd(B) += 1;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Total degenerate triangles: %d"), DegenerateTriangleCount);

    int32 NonManifoldEdgeCount = 0;
    for (const auto& Pair : EdgeUseCount)
    {
        if (Pair.Value > 2)
        {
            UE_LOG(LogTemp, Warning, TEXT("Non-manifold edge (%d, %d) used %d times"),
                Pair.Key.Key, Pair.Key.Value, Pair.Value);
            NonManifoldEdgeCount++;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Total non-manifold edges: %d"), NonManifoldEdgeCount);

    int32 HighValenceVertexCount = 0;
    for (const auto& Pair : VertexEdgeUseCount)
    {
        if (Pair.Value > 10)
        {
            UE_LOG(LogTemp, Warning, TEXT("High valence vertex %d used in %d edges"),
                Pair.Key, Pair.Value);
            HighValenceVertexCount++;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Total high valence vertices: %d"), HighValenceVertexCount);
}

void  ULimbCleanerTool::RemoveLooseBridges(TArray<FVector3f>& Vertices, TArray<FIntVector>& Triangles, float MaxBridgeLength = 50.f);
{
    TSet<int32> BridgeTriangles;

    for (int32 TriIndex = 0; TriIndex < Triangles.Num(); ++TriIndex)
    {
        const FIntVector& Tri = Triangles[TriIndex];
        FVector3f A = Vertices[Tri.X];
        FVector3f B = Vertices[Tri.Y];
        FVector3f C = Vertices[Tri.Z];

        // Check all three edges of the triangle
        float AB = FVector3f::Dist(A, B);
        float BC = FVector3f::Dist(B, C);
        float CA = FVector3f::Dist(C, A);

        // If all three are below threshold, keep the triangle
        // If any one edge is too long, it's potentially a bridge
        if (AB > MaxBridgeLength || BC > MaxBridgeLength || CA > MaxBridgeLength)
        {
            BridgeTriangles.Add(TriIndex);
        }
    }

    if (BridgeTriangles.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Removed %d potentially bridging triangles"), BridgeTriangles.Num());
    }

    // Remove the bridge triangles from the triangle list
    Triangles.RemoveAll([&](const FIntVector& Tri, int32 Index)
        {
            return BridgeTriangles.Contains(Index);
        });
}