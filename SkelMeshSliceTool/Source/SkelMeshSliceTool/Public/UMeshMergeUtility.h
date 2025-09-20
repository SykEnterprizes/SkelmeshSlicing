// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "FRawVertexData.h"

#include "FMeshBuildContext.h"

#include </Program Files/Epic Games/UE_5.0/Engine/Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h>

#include "ESkinningType.h"

#include "FCapSkinningInfo.h"

#include "UMeshMergeUtility.generated.h"

struct FRawMeshData
{
	TArray<FRawVertexData> Vertices; // i need to change this, its awkward to extract from
	TArray<FVector2D> UVs;
	TArray<FVector> Normals;
	TArray<FVector> Tangents;
	TArray<FColor> Colors;
	TArray<FIntVector> Triangles;
	TArray<int32> TriangleMaterialIndices; // One entry per triangle
};

// Welding utility structure to hold welded mesh data
struct FWeldedMeshData
{
	TArray<FVector> WeldedPositions;
	TArray<FVector> WeldedNormals;
	TArray<FVector> WeldedTangents;
	TArray<FVector2D> WeldedUVs;
	TArray<FColor> WeldedColors;
	TArray<FRawVertexData> WeldedRawVertexData;
	TArray<FIntVector> WeldedTriangles;
	TArray<int32> VertexRemapping; // Maps original vertex index to welded vertex index

	int32 GetWeldedVertexCount() const { return WeldedPositions.Num(); }
};
/**
 * 
 */
UCLASS()
class SKELMESHSLICETOOL_API UUMeshMergeUtility : public UObject
{
	GENERATED_BODY()
	
public:
    
	void ConvertProceduralMeshToRawMeshData(TArray<UProceduralMeshComponent*> ProcMesh,	FRawMeshData& OutRaw);

	void ExtractRawMeshDataFromSkeletalMesh(USkeletalMesh* SkeletalMesh, FRawMeshData& OutRaw);

	void ExtractRawMeshDataFromCachedMeshContext(FMeshBuildContext Context, FRawMeshData& OutRaw);

	void ExtractCapRawMesh(
		UProceduralMeshComponent* CapComp,
		TArray<FRawVertexData>& OutVertices,
		TArray<FIntVector>& OutTriangles,
		TArray<FVector2D>& OutUVs,
		TArray<FVector>& OutNormals,
		TArray<FVector>& OutTangents,
		TArray<FColor>& OutColors,
		TArray<int32>& OutTriangleMaterialIndices
	);

	void BuildFinalMeshDataFromSections(const FRawMeshData& InputRaw, const TMap<int32, TArray<FIntVector>>& SectionTriangles, const TArray<FSkeletalMaterial>& FinalMaterials, FRawMeshData& OutFinalRaw, TArray<int32>& OutTriangleMaterialIndices);

	FWeldedMeshData WeldDuplicateVertices(const FRawMeshData& Raw, const TArray<FRawVertexData>& RawV, float WeldThreshold);

	FWeldedMeshData PopulateMeshDataWithoutWelding(const FRawMeshData& Raw, const TArray<FRawVertexData>& RawV);

	void BuildSkeletalMeshFromRawMeshDataMultiMaterialWelded(const FRawMeshData& Raw, const TArray<FRawVertexData>& RawV, const TArray<FSkeletalMaterial>& Materials, const TArray<int32>& TriangleMaterialIndices, USkeletalMesh* SkeletalMesh, const TMap<int32, FCapSkinningInfo>& CapSkinningInfos, float WeldThreshold);

	void BuildStaticMeshFromRawMeshDataMultiMaterialWelded(const FRawMeshData& Raw, const TArray<FRawVertexData>& RawV, const TArray<FSkeletalMaterial>& Materials, const TArray<int32>& TriangleMaterialIndices, UStaticMesh* FinalMesh, const TMap<int32, FCapSkinningInfo>& CapSkinningInfos, float WeldThreshold);

	void BuildStaticMeshFromProceduralMeshWelded(const FRawMeshData& Raw, UMaterialInterface*& Materials,	const TArray<int32>& TriangleMaterialIndices, UStaticMesh* FinalMesh, float WeldThreshold);

	void BuildStaticMeshFromReverseCaps(int32 index, TMap<int32, UProceduralMeshComponent*> ReverseCaps);
	
	// for the collection of data from a skelmesh and the capcomponents to build a final mesh
	void GenerateMergedMeshData(const TArray<UProceduralMeshComponent*>& CapMeshComps, USkeletalMesh* SourceMesh, FRawMeshData& OutBaseRaw, FRawMeshData& OutMergedRaw, TArray<FSkeletalMaterial>& OutFinalMaterials, TMap<UMaterialInterface*, int32>& OutMaterialToIndex, TMap<int32, TArray<FIntVector>>& OutSectionTriangles, TMap<int32, FIntPoint>& OutSectionVertexRanges, TMap<int32, FCapSkinningInfo>& CapSkinningInfos);
	// for the collection of data from a ststic mesh and the capcomponents to build a final mesh
	void GenerateMergedStaticMeshData(const TArray<UProceduralMeshComponent*>& CapMeshComps, const FMeshBuildContext& Context, FRawMeshData& OutMergedRaw, TArray<FSkeletalMaterial>& OutFinalMaterials, TMap<UMaterialInterface*, int32>& OutMaterialToIndex, TMap<int32, TArray<FIntVector>>& OutSectionTriangles, TMap<int32, FIntPoint>& OutSectionVertexRanges, TMap<int32, FCapSkinningInfo>& CapSkinningInfos);

	void ProcessBaseMeshTriangles(const FMeshBuildContext& Context, const FRawMeshData& BaseRaw, const TMap<UMaterialInterface*, int32>& MaterialToFinalIndex, TMap<int32, TArray<FIntVector>>& OutSectionTriangles, TMap<int32, FIntPoint>& OutSectionVertexRanges);

	void BuildSkeletalMeshFromRawMeshDataMultiMaterial(/*const*/ FRawMeshData& Raw, const TArray<FRawVertexData>& RawV, const TArray<FSkeletalMaterial>& Materials, const TArray<int32>& TriangleMaterialIndices, USkeletalMesh* SkeletalMesh, const TMap<int32, FCapSkinningInfo>& CapSkinningInfos);

	void BlendVertexInfluences(FRawVertexData& ExistingVert, const FRawVertexData& NewVert);

	void MergeMultipleCapsIntoProceduralMesh(
		const TArray<UProceduralMeshComponent*>& CapMeshComps,
		const FMeshBuildContext& Context,
		UProceduralMeshComponent* FinalMesh, // not needed because we have the cachedmeshcontext to draw from
		const TMap<int32, FCapSkinningInfo>& Infos, bool WeldMeshes);

	void MergeMultipleCapsIntoSkeletalMesh(
		const TArray<UProceduralMeshComponent*>& CapMeshComps,
		const TArray<FRawVertexData>& OriginalSkelMeshCompData,
		USkeletalMesh*& OutLimbSkelMesh,
		const TMap<int32, FCapSkinningInfo>& Infos, bool WeldMeshes = true);

	void PreviewMeshInPersona(UObject* Mesh);

	void UpdateCapSkinningIndicesAfterWelding(TMap<int32, FCapSkinningInfo>& CapSkinningInfos, const FWeldedMeshData& WeldedData);
	
	void AutoSkinCapVerticesWithIndices(const FReferenceSkeleton& RefSkeleton, const TArray<FRawVertexData>& RawVertexData, const TArray<FVector>& Vertices, FSkeletalMeshLODModel& LODModel, FSkelMeshSection& Section, const TMap<int32, FCapSkinningInfo>& CapSkinningInfos, const FWeldedMeshData& WeldedData, int32 MaxInfluences);
	
	void ApplyCapSpecificSkinning(FSoftSkinVertex& Vertex, int32 VertexIndex, const FCapSkinningInfo& CapInfo, int32 CapIndex, const FReferenceSkeleton& RefSkeleton, const TArray<FRawVertexData>& RawVertexData, const TArray<FVector>& Vertices, const FSkelMeshSection& Section, const TMap<int32, int32>& BoneToCompact, int32 AuthoredVerts, int32 MaxInfluences);

private:

};

