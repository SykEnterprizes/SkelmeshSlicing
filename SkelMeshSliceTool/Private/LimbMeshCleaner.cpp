
// LimbMeshCleaner.cpp
#include "LimbMeshCleaner.h"
#include "Containers/Set.h"
#include "Logging/LogMacros.h"

void UMeshCleanUtils::UtilCleanMeshForCapping(
    TArray<FVector3f>& Vertices,
    TArray<FIntVector>& Triangles, TArray<FVector2D>& InUVs, TArray<FVector>& InNormals,
    float WeldThreshold,
    int32 ValenceThreshold, bool RunDiagnostic)
{
    WeldVertices(Vertices, Triangles, InUVs, InNormals, 0.01f);
    RemoveDegenerateTriangles(Triangles);
    RemoveDuplicateTriangles(Triangles);
    RemoveNonManifoldTriangles(Triangles);
    if(RunDiagnostic)
    {
        DiagnoseMeshIntegrity(Vertices, Triangles, .01);
    }
}

void UMeshCleanUtils::WeldVertices(
    TArray<FVector3f>& Vertices,
    TArray<FIntVector>& Triangles,
    TArray<FVector2D>& InOutUVs,
    TArray<FVector>& InOutNormals,
    float WeldThreshold)
{
    
    TMap<int32, int32> Remap;
    TArray<FVector3f> UniqueVerts;
    TArray<FVector2D> WeldedUVs;
    TArray<FVector> WeldedNormals;

    int32 UVCount = InOutUVs.Num();
    int32 NormalCount = InOutNormals.Num();

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

        const FVector2D UV = (i < UVCount) ? InOutUVs[i] : FVector2D::ZeroVector;
        const FVector Normal = (i < NormalCount) ? InOutNormals[i] : FVector::UpVector;

        if (FoundIndex != INDEX_NONE)
        {
            Remap.Add(i, FoundIndex);
        }
        else
        {
            int32 NewIndex = UniqueVerts.Add(V);
            WeldedUVs.Add(UV);
            WeldedNormals.Add(Normal);
            Remap.Add(i, NewIndex);
        }
    }

    // Apply to triangles
    for (FIntVector& Tri : Triangles)
    {
        Tri.X = Remap[Tri.X];
        Tri.Y = Remap[Tri.Y];
        Tri.Z = Remap[Tri.Z];
    }

    Vertices = MoveTemp(UniqueVerts);
    InOutUVs = MoveTemp(WeldedUVs);
    InOutNormals = MoveTemp(WeldedNormals);
}

void UMeshCleanUtils::RemoveDegenerateTriangles(
    TArray<FIntVector>& Triangles)
{
    Triangles.RemoveAll([](const FIntVector& Tri)
    {
        return Tri.X == Tri.Y || Tri.Y == Tri.Z || Tri.Z == Tri.X;
    });
}

void UMeshCleanUtils::RemoveDuplicateTriangles(TArray<FIntVector>& Triangles)
{
    TSet<FIntVector> UniqueTris;
    TArray<FIntVector> Filtered;

    for (const FIntVector& Tri : Triangles)
    {
        // Extract and sort manually
        int32 A = Tri.X;
        int32 B = Tri.Y;
        int32 C = Tri.Z;

        // Sort the indices manually (stable)
        if (A > B) Swap(A, B);
        if (B > C) Swap(B, C);
        if (A > B) Swap(A, B);

        FIntVector SortedTri(A, B, C);

        if (!UniqueTris.Contains(SortedTri))
        {
            UniqueTris.Add(SortedTri);
            Filtered.Add(Tri); // Retain original winding
        }
    }

    Triangles = MoveTemp(Filtered);
}

void UMeshCleanUtils::RemoveNonManifoldTriangles(TArray<FIntVector>& Triangles)
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

void UMeshCleanUtils::DiagnoseMeshIntegrity(const TArray<FVector3f>& Vertices,
    const TArray<FIntVector>& Triangles,
    int32 ValenceThreshold)
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

void  UMeshCleanUtils::RemoveLooseBridges(TArray<FVector3f>& Vertices, TArray<FIntVector>& Triangles, float MaxBridgeLength)
{
    TSet<int32> BridgeTriangles;

    for (int32 TriIndex = 0; TriIndex < Triangles.Num(); ++TriIndex)
    {
        const FIntVector& Tri = Triangles[TriIndex];
        const FVector3f& A = Vertices[Tri.X];
        const FVector3f& B = Vertices[Tri.Y];
        const FVector3f& C = Vertices[Tri.Z];

        float AB = FVector3f::Dist(A, B);
        float BC = FVector3f::Dist(B, C);
        float CA = FVector3f::Dist(C, A);

        if (AB > MaxBridgeLength || BC > MaxBridgeLength || CA > MaxBridgeLength)
        {
            BridgeTriangles.Add(TriIndex);
        }
    }

    if (BridgeTriangles.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Removed %d potentially bridging triangles"), BridgeTriangles.Num());
    }

    // Create a new array for valid triangles
    TArray<FIntVector> FilteredTriangles;
    FilteredTriangles.Reserve(Triangles.Num() - BridgeTriangles.Num());

    for (int32 TriIndex = 0; TriIndex < Triangles.Num(); ++TriIndex)
    {
        if (!BridgeTriangles.Contains(TriIndex))
        {
            FilteredTriangles.Add(Triangles[TriIndex]);
        }
    }

    // Replace old array
    Triangles = MoveTemp(FilteredTriangles);
}

