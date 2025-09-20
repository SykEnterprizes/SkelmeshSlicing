// Edge Loop Clustering System for Complex Mesh Extractions
// Handles overlapping, adjacent, and fragmented edge loops from bone weight extractions

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

#include "UEdgeLoopClusteringUtility.generated.h"

// Data structures
struct FEdgeLoop
{
    TArray<FVector> Vertices;
    FVector Centroid;
    float Radius;
    float Area;
    int32 OriginalMeshSection; // Track which mesh section this came from
    bool bIsComplete; // Whether this forms a closed loop

    FEdgeLoop()
    {
        Centroid = FVector::ZeroVector;
        Radius = 0.0f;
        Area = 0.0f;
        OriginalMeshSection = -1;
        bIsComplete = false;
    }

    void CalculateProperties()
    {
        if (Vertices.Num() < 3) return;

        // Calculate centroid
        Centroid = FVector::ZeroVector;
        for (const FVector& Vertex : Vertices)
        {
            Centroid += Vertex;
        }
        Centroid /= Vertices.Num();

        // Calculate radius (max distance from centroid)
        Radius = 0.0f;
        for (const FVector& Vertex : Vertices)
        {
            float Distance = FVector::Dist(Centroid, Vertex);
            Radius = FMath::Max(Radius, Distance);
        }

        // Estimate area using simple polygon area calculation
        Area = 0.0f;
        for (int32 i = 0; i < Vertices.Num(); ++i)
        {
            int32 NextIndex = (i + 1) % Vertices.Num();
            FVector V1 = Vertices[i] - Centroid;
            FVector V2 = Vertices[NextIndex] - Centroid;
            Area += FVector::CrossProduct(V1, V2).Size() * 0.5f;
        }

        // Check if loop is reasonably complete (vertices form a closed shape)
        if (Vertices.Num() >= 3)
        {
            float FirstToLastDist = FVector::Dist(Vertices[0], Vertices.Last());
            float AvgEdgeLength = 0.0f;
            for (int32 i = 0; i < Vertices.Num() - 1; ++i)
            {
                AvgEdgeLength += FVector::Dist(Vertices[i], Vertices[i + 1]);
            }
            AvgEdgeLength /= (Vertices.Num() - 1);

            // Consider complete if first-to-last distance is reasonable compared to average edge length
            bIsComplete = FirstToLastDist <= AvgEdgeLength * 2.0f;
        }
    }
};

enum class ELoopRelationship
{
    Separate,     // Distinct loops that shouldn't be merged
    Adjacent,     // Should be bridged together
    Concentric,   // One inside another (clothing over body)
    Overlapping,  // Partial overlap, needs merging
    Fragment      // Small fragment that should be absorbed
};

USTRUCT(BlueprintType)
struct FEdgeLoopCluster
{
	GENERATED_BODY()

public:

    TArray<FEdgeLoop> Loops;
    FVector ClusterCentroid;
    float ClusterRadius;
    int32 DominantLoopIndex; // Index of the largest/most complete loop

    FEdgeLoopCluster()
    {
        ClusterCentroid = FVector::ZeroVector;
        ClusterRadius = 0.0f;
        DominantLoopIndex = -1;
    }

    void CalculateClusterProperties()
    {
        if (Loops.Num() == 0) return;

        // Calculate cluster centroid
        ClusterCentroid = FVector::ZeroVector;
        for (const FEdgeLoop& Loop : Loops)
        {
            ClusterCentroid += Loop.Centroid;
        }
        ClusterCentroid /= Loops.Num();

        // Find cluster radius and dominant loop
        ClusterRadius = 0.0f;
        float MaxArea = 0.0f;

        for (int32 i = 0; i < Loops.Num(); ++i)
        {
            const FEdgeLoop& Loop = Loops[i];
            float DistanceFromCenter = FVector::Dist(ClusterCentroid, Loop.Centroid) + Loop.Radius;
            ClusterRadius = FMath::Max(ClusterRadius, DistanceFromCenter);

            // Find dominant loop (largest area and most complete)
            float LoopScore = Loop.Area * (Loop.bIsComplete ? 1.5f : 1.0f);
            if (LoopScore > MaxArea)
            {
                MaxArea = LoopScore;
                DominantLoopIndex = i;
            }
        }
    }
};

UCLASS()
class SKELMESHSLICETOOL_API UEdgeLoopClusteringUtility : public UObject
{
	GENERATED_BODY()

public:

    // Main clustering function
    static TArray<FEdgeLoopCluster> ClusterEdgeLoops(const TArray<FEdgeLoop>& InputLoops, float ClusterRadius = 5.0f)
    {
        TArray<FEdgeLoopCluster> Clusters;
        TArray<bool> ProcessedLoops;
        ProcessedLoops.SetNum(InputLoops.Num());

        for (int32 i = 0; i < InputLoops.Num(); ++i)
        {
            if (ProcessedLoops[i]) continue;

            // Start new cluster
            FEdgeLoopCluster NewCluster;
            NewCluster.Loops.Add(InputLoops[i]);
            ProcessedLoops[i] = true;

            // Find nearby loops to add to this cluster
            for (int32 j = i + 1; j < InputLoops.Num(); ++j)
            {
                if (ProcessedLoops[j]) continue;

                float Distance = FVector::Dist(InputLoops[i].Centroid, InputLoops[j].Centroid);
                if (Distance <= ClusterRadius)
                {
                    NewCluster.Loops.Add(InputLoops[j]);
                    ProcessedLoops[j] = true;
                }
            }

            NewCluster.CalculateClusterProperties();
            Clusters.Add(NewCluster);
        }

        return Clusters;
    }

    // Determine relationship between two loops in a cluster
    static ELoopRelationship AnalyzeLoopRelationship(const FEdgeLoop& Loop1, const FEdgeLoop& Loop2)
    {
        float CentroidDistance = FVector::Dist(Loop1.Centroid, Loop2.Centroid);
        float CombinedRadius = Loop1.Radius + Loop2.Radius;

        // If centroids are far apart relative to their sizes, they're separate
        if (CentroidDistance > CombinedRadius * 1.5f)
        {
            return ELoopRelationship::Separate;
        }

        // Check if one loop is much smaller (potential fragment)
        float AreaRatio = FMath::Max(Loop1.Area, Loop2.Area) / FMath::Max(FMath::Min(Loop1.Area, Loop2.Area), 0.001f);
        if (AreaRatio > 10.0f)
        {
            return ELoopRelationship::Fragment;
        }

        // Check for concentricity (one inside another)
        if (CentroidDistance < FMath::Abs(Loop1.Radius - Loop2.Radius) * 0.8f)
        {
            return ELoopRelationship::Concentric;
        }

        // Check for overlap
        if (CentroidDistance < (Loop1.Radius + Loop2.Radius) * 0.7f)
        {
            return ELoopRelationship::Overlapping;
        }

        // Default to adjacent (should be bridged)
        return ELoopRelationship::Adjacent;
    }

    // Merge loops within a cluster based on their relationships
    static FEdgeLoop MergeClusterLoops(const FEdgeLoopCluster& Cluster)
    {
        if (Cluster.Loops.Num() == 0) return FEdgeLoop();
        if (Cluster.Loops.Num() == 1) return Cluster.Loops[0];

        // Start with the dominant loop
        FEdgeLoop MergedLoop = Cluster.Loops[Cluster.DominantLoopIndex];

        for (int32 i = 0; i < Cluster.Loops.Num(); ++i)
        {
            if (i == Cluster.DominantLoopIndex) continue;

            const FEdgeLoop& OtherLoop = Cluster.Loops[i];
            ELoopRelationship Relationship = AnalyzeLoopRelationship(MergedLoop, OtherLoop);

            switch (Relationship)
            {
            case ELoopRelationship::Fragment:
            case ELoopRelationship::Overlapping:
                // Merge vertices from the other loop
                MergeVerticesIntoLoop(MergedLoop, OtherLoop);
                break;

            case ELoopRelationship::Adjacent:
                // Could bridge these, but for now just merge
                MergeVerticesIntoLoop(MergedLoop, OtherLoop);
                break;

            case ELoopRelationship::Concentric:
                // Keep the outer loop (larger radius)
                if (OtherLoop.Radius > MergedLoop.Radius)
                {
                    MergedLoop = OtherLoop;
                }
                break;

            case ELoopRelationship::Separate:
            default:
                // Don't merge separate loops
                break;
            }
        }

        // Recalculate properties after merging
        MergedLoop.CalculateProperties();
        return MergedLoop;
    }

    // Filter out very small or invalid loops
    static TArray<FEdgeLoop> FilterLoops(const TArray<FEdgeLoop>& InputLoops, float MinArea = 1.0f, int32 MinVertices = 3)
    {
        TArray<FEdgeLoop> FilteredLoops;

        for (const FEdgeLoop& Loop : InputLoops)
        {
            if (Loop.Vertices.Num() >= MinVertices && Loop.Area >= MinArea)
            {
                FilteredLoops.Add(Loop);
            }
        }

        return FilteredLoops;
    }

private:

    // Helper function to merge vertices from one loop into another
    static void MergeVerticesIntoLoop(FEdgeLoop& TargetLoop, const FEdgeLoop& SourceLoop)
    {
        // Simple approach: add all vertices and let later processing clean up duplicates
        // More sophisticated approach would involve proper curve merging

        for (const FVector& Vertex : SourceLoop.Vertices)
        {
            // Check if this vertex is too close to existing vertices
            bool bIsDuplicate = false;
            for (const FVector& ExistingVertex : TargetLoop.Vertices)
            {
                if (FVector::Dist(Vertex, ExistingVertex) < 0.1f) // 1mm tolerance
                {
                    bIsDuplicate = true;
                    break;
                }
            }

            if (!bIsDuplicate)
            {
                TargetLoop.Vertices.Add(Vertex);
            }
        }

        // Sort vertices to maintain proper loop order
        SortVerticesInLoop(TargetLoop);
    }

    // Sort vertices to form a proper loop (simplified version)
    static void SortVerticesInLoop(FEdgeLoop& Loop)
    {
        if (Loop.Vertices.Num() < 3) return;

        // Simple approach: sort by angle from centroid
        FVector Center = Loop.Centroid;
        Loop.Vertices.Sort([Center](const FVector& A, const FVector& B)
            {
                FVector DirA = (A - Center).GetSafeNormal();
                FVector DirB = (B - Center).GetSafeNormal();
                float AngleA = FMath::Atan2(DirA.Y, DirA.X);
                float AngleB = FMath::Atan2(DirB.Y, DirB.X);
                return AngleA < AngleB;
            });
    }
};

//// Example usage function
//void ProcessComplexMeshEdgeLoops(const TArray<FEdgeLoop>& RawEdgeLoops)
//{
//    UE_LOG(LogTemp, Warning, TEXT("Processing %d raw edge loops"), RawEdgeLoops.Num());
//
//    // Step 1: Filter out tiny/invalid loops
//    TArray<FEdgeLoop> FilteredLoops = UEdgeLoopClusteringUtility::FilterLoops(RawEdgeLoops, 0.5f, 3);
//    UE_LOG(LogTemp, Warning, TEXT("After filtering: %d loops"), FilteredLoops.Num());
//
//    // Step 2: Cluster nearby loops
//    TArray<FEdgeLoopCluster> Clusters = UEdgeLoopClusteringUtility::ClusterEdgeLoops(FilteredLoops, 5.0f);
//    UE_LOG(LogTemp, Warning, TEXT("Created %d clusters"), Clusters.Num());
//
//    // Step 3: Merge loops within each cluster
//    TArray<FEdgeLoop> FinalLoops;
//    for (const FEdgeLoopCluster& Cluster : Clusters)
//    {
//        FEdgeLoop MergedLoop = UEdgeLoopClusteringUtility::MergeClusterLoops(Cluster);
//        if (MergedLoop.Vertices.Num() >= 3)
//        {
//            FinalLoops.Add(MergedLoop);
//        }
//
//        UE_LOG(LogTemp, Warning, TEXT("Cluster with %d loops merged into 1 loop with %d vertices"),
//            Cluster.Loops.Num(), MergedLoop.Vertices.Num());
//    }
//
//    UE_LOG(LogTemp, Warning, TEXT("Final result: %d merged edge loops"), FinalLoops.Num());
//}
//
