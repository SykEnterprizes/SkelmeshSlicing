// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SkelMeshSliceTool/Public/FEdgeLoopInfo.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFEdgeLoopInfo() {}
// Cross Module References
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FEdgeLoopInfo();
	UPackage* Z_Construct_UPackage__Script_SkelMeshSliceTool();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector3f();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
	ENGINE_API UClass* Z_Construct_UClass_UMaterialInterface_NoRegister();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_EdgeLoopInfo;
class UScriptStruct* FEdgeLoopInfo::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_EdgeLoopInfo.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_EdgeLoopInfo.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FEdgeLoopInfo, Z_Construct_UPackage__Script_SkelMeshSliceTool(), TEXT("EdgeLoopInfo"));
	}
	return Z_Registration_Info_UScriptStruct_EdgeLoopInfo.OuterSingleton;
}
template<> SKELMESHSLICETOOL_API UScriptStruct* StaticStruct<FEdgeLoopInfo>()
{
	return FEdgeLoopInfo::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FIntPropertyParams NewProp_VertexIndices_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_VertexIndices_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_VertexIndices;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Positions_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Positions_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_Positions;
		static const UECodeGen_Private::FStructPropertyParams NewProp_FadeRingPositions_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FadeRingPositions_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_FadeRingPositions;
		static const UECodeGen_Private::FStructPropertyParams NewProp_UVs_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_UVs_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_UVs;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_VotedMaterialIndex_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_VotedMaterialIndex;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TotalEdgeLength_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TotalEdgeLength;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bAddFadeRing_MetaData[];
#endif
		static void NewProp_bAddFadeRing_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAddFadeRing;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bAddDome_MetaData[];
#endif
		static void NewProp_bAddDome_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAddDome;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bAddNoise_MetaData[];
#endif
		static void NewProp_bAddNoise_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAddNoise;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bFlatten_MetaData[];
#endif
		static void NewProp_bFlatten_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bFlatten;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bUseManualNormal_MetaData[];
#endif
		static void NewProp_bUseManualNormal_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bUseManualNormal;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LoopNumber_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_LoopNumber;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DomeHeight_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_DomeHeight;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FadeRingHieght_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_FadeRingHieght;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RingCount_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_RingCount;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_NoiseAmount_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_NoiseAmount;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_UV_Rotation_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_UV_Rotation;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_UV_Scale_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_UV_Scale;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_UV_Offset_X_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_UV_Offset_X;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_UV_Offset_Y_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_UV_Offset_Y;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TwistFixOverride_MetaData[];
#endif
		static void NewProp_TwistFixOverride_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_TwistFixOverride;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PlanitaryThreshold_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_PlanitaryThreshold;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxNormalDeviation_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxNormalDeviation;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SegmentSlopeTolerance_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SegmentSlopeTolerance;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SegmentWindow_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_SegmentWindow;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CapMaterial_MetaData[];
#endif
		static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_CapMaterial;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FadeRingMaterial_MetaData[];
#endif
		static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_FadeRingMaterial;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AssociatedBone_MetaData[];
#endif
		static const UECodeGen_Private::FTextPropertyParams NewProp_AssociatedBone;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LimbEndCollapseAmount_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_LimbEndCollapseAmount;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReverseCapFadeAmount_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReverseCapFadeAmount;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RevCapExpansionAmount_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RevCapExpansionAmount;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RevCapFadeRingCount_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_RevCapFadeRingCount;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FadeRingCount_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_FadeRingCount;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_UV_RCFRScale_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_UV_RCFRScale;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_UV_RCFROffset_X_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_UV_RCFROffset_X;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_UV_RCFROffset_Y_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_UV_RCFROffset_Y;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_UV_RCFRRotation_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_UV_RCFRRotation;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FEdgeLoopInfo>();
	}
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VertexIndices_Inner = { "VertexIndices", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VertexIndices_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VertexIndices = { "VertexIndices", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, VertexIndices), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VertexIndices_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VertexIndices_MetaData)) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_Positions_Inner = { "Positions", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FVector3f, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_Positions_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// vertiex indices are the indices from the output sorted edgeloop array\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "vertiex indices are the indices from the output sorted edgeloop array" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_Positions = { "Positions", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, Positions), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_Positions_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_Positions_MetaData)) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingPositions_Inner = { "FadeRingPositions", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FVector3f, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingPositions_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// the actual vectors of the edgeloop vertices\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "the actual vectors of the edgeloop vertices" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingPositions = { "FadeRingPositions", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, FadeRingPositions), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingPositions_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingPositions_MetaData)) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UVs_Inner = { "UVs", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UVs_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// the actual vectors of the generated fadering vertices\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "the actual vectors of the generated fadering vertices" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UVs = { "UVs", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, UVs), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UVs_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UVs_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VotedMaterialIndex_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// the uvs assigned to the edgeloops\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "the uvs assigned to the edgeloops" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VotedMaterialIndex = { "VotedMaterialIndex", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, VotedMaterialIndex), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VotedMaterialIndex_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VotedMaterialIndex_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TotalEdgeLength_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// the material index designated from the base mesh\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "the material index designated from the base mesh" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TotalEdgeLength = { "TotalEdgeLength", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, TotalEdgeLength), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TotalEdgeLength_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TotalEdgeLength_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddFadeRing_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// unused yet.. but maybe good for something later\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "unused yet.. but maybe good for something later" },
	};
#endif
	void Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddFadeRing_SetBit(void* Obj)
	{
		((FEdgeLoopInfo*)Obj)->bAddFadeRing = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddFadeRing = { "bAddFadeRing", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FEdgeLoopInfo), &Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddFadeRing_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddFadeRing_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddFadeRing_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddDome_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// add a fade ring to help with excessively uneven edgeloops\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "add a fade ring to help with excessively uneven edgeloops" },
	};
#endif
	void Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddDome_SetBit(void* Obj)
	{
		((FEdgeLoopInfo*)Obj)->bAddDome = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddDome = { "bAddDome", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FEdgeLoopInfo), &Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddDome_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddDome_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddDome_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddNoise_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// add a dome shape, or just a fan cap\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "add a dome shape, or just a fan cap" },
	};
#endif
	void Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddNoise_SetBit(void* Obj)
	{
		((FEdgeLoopInfo*)Obj)->bAddNoise = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddNoise = { "bAddNoise", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FEdgeLoopInfo), &Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddNoise_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddNoise_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddNoise_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bFlatten_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// toggles adding noise to the procedural generation\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "toggles adding noise to the procedural generation" },
	};
#endif
	void Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bFlatten_SetBit(void* Obj)
	{
		((FEdgeLoopInfo*)Obj)->bFlatten = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bFlatten = { "bFlatten", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FEdgeLoopInfo), &Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bFlatten_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bFlatten_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bFlatten_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bUseManualNormal_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// toggles flat slicing the cap generation\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "toggles flat slicing the cap generation" },
	};
#endif
	void Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bUseManualNormal_SetBit(void* Obj)
	{
		((FEdgeLoopInfo*)Obj)->bUseManualNormal = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bUseManualNormal = { "bUseManualNormal", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FEdgeLoopInfo), &Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bUseManualNormal_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bUseManualNormal_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bUseManualNormal_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_LoopNumber_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_LoopNumber = { "LoopNumber", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, LoopNumber), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_LoopNumber_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_LoopNumber_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_DomeHeight_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// an identifier for the loop  **** This shouldnt be changed since its an identifier, and if we break a struct in bp, unless its filled exactly, will cause issues with other loops\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "an identifier for the loop  **** This shouldnt be changed since its an identifier, and if we break a struct in bp, unless its filled exactly, will cause issues with other loops" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_DomeHeight = { "DomeHeight", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, DomeHeight), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_DomeHeight_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_DomeHeight_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingHieght_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// height of the dome, if applicable\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "height of the dome, if applicable" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingHieght = { "FadeRingHieght", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, FadeRingHieght), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingHieght_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingHieght_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RingCount_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// hieght of the fadering, if applicable\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "hieght of the fadering, if applicable" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RingCount = { "RingCount", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, RingCount), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RingCount_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RingCount_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_NoiseAmount_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// number of inner rings to add to the fade ring\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "number of inner rings to add to the fade ring" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_NoiseAmount = { "NoiseAmount", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, NoiseAmount), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_NoiseAmount_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_NoiseAmount_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Rotation_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// amount of noise to apply to the cap\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "amount of noise to apply to the cap" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Rotation = { "UV_Rotation", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, UV_Rotation), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Rotation_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Rotation_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Scale_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// amount of rotation we can add to the UV assignment of the cap or dome\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "amount of rotation we can add to the UV assignment of the cap or dome" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Scale = { "UV_Scale", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, UV_Scale), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Scale_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Scale_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Offset_X_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// amount of scale we can ad to the UV assignment of the cap or dome\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "amount of scale we can ad to the UV assignment of the cap or dome" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Offset_X = { "UV_Offset_X", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, UV_Offset_X), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Offset_X_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Offset_X_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Offset_Y_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// amount of X offset we can assign to the UV in the cap or dome\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "amount of X offset we can assign to the UV in the cap or dome" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Offset_Y = { "UV_Offset_Y", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, UV_Offset_Y), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Offset_Y_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Offset_Y_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TwistFixOverride_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// amount of Y offset we can assign the UV in the cap or dome\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "amount of Y offset we can assign the UV in the cap or dome" },
	};
#endif
	void Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TwistFixOverride_SetBit(void* Obj)
	{
		((FEdgeLoopInfo*)Obj)->TwistFixOverride = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TwistFixOverride = { "TwistFixOverride", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FEdgeLoopInfo), &Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TwistFixOverride_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TwistFixOverride_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TwistFixOverride_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_PlanitaryThreshold_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// override for the loop twist-risk checker\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "override for the loop twist-risk checker" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_PlanitaryThreshold = { "PlanitaryThreshold", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, PlanitaryThreshold), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_PlanitaryThreshold_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_PlanitaryThreshold_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_MaxNormalDeviation_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// Max allowed vertical offset between loop points from a best-fit plane.  High = tolerant of warped loops.  Low = flags even mild bends \n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "Max allowed vertical offset between loop points from a best-fit plane.  High = tolerant of warped loops.  Low = flags even mild bends" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_MaxNormalDeviation = { "MaxNormalDeviation", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, MaxNormalDeviation), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_MaxNormalDeviation_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_MaxNormalDeviation_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_SegmentSlopeTolerance_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// Allowed angle (in degrees) between segment normals and expected orientation.  Controls how much normal flipping is considered ?twisted.? Great for detecting shading artifacts.\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "Allowed angle (in degrees) between segment normals and expected orientation.  Controls how much normal flipping is considered ?twisted.? Great for detecting shading artifacts." },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_SegmentSlopeTolerance = { "SegmentSlopeTolerance", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, SegmentSlopeTolerance), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_SegmentSlopeTolerance_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_SegmentSlopeTolerance_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_SegmentWindow_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "//  Acceptable slope variance between loop segments.  Helps catch loops with uneven contour, which may hint at visual twisting.\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "Acceptable slope variance between loop segments.  Helps catch loops with uneven contour, which may hint at visual twisting." },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_SegmentWindow = { "SegmentWindow", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, SegmentWindow), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_SegmentWindow_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_SegmentWindow_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_CapMaterial_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "//  Number of segments to include in each local comparison window.  Smaller = detects tight twists. Larger = favors broader averaging.\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "Number of segments to include in each local comparison window.  Smaller = detects tight twists. Larger = favors broader averaging." },
	};
#endif
	const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_CapMaterial = { "CapMaterial", nullptr, (EPropertyFlags)0x0014000000000004, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, CapMaterial), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_CapMaterial_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_CapMaterial_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingMaterial_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// material to apply to the cap\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "material to apply to the cap" },
	};
#endif
	const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingMaterial = { "FadeRingMaterial", nullptr, (EPropertyFlags)0x0014000000000004, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, FadeRingMaterial), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingMaterial_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingMaterial_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_AssociatedBone_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// material to apply to the fadering ** optional and tbd since we already get it from InferMaterialFromPositions()** \n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "material to apply to the fadering ** optional and tbd since we already get it from InferMaterialFromPositions()**" },
	};
#endif
	const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_AssociatedBone = { "AssociatedBone", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, AssociatedBone), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_AssociatedBone_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_AssociatedBone_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_LimbEndCollapseAmount_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// give the loop an associated bone to do normal direction calc\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "give the loop an associated bone to do normal direction calc" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_LimbEndCollapseAmount = { "LimbEndCollapseAmount", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, LimbEndCollapseAmount), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_LimbEndCollapseAmount_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_LimbEndCollapseAmount_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_ReverseCapFadeAmount_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// the amount the fadering will collapse by\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "the amount the fadering will collapse by" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_ReverseCapFadeAmount = { "ReverseCapFadeAmount", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, ReverseCapFadeAmount), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_ReverseCapFadeAmount_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_ReverseCapFadeAmount_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RevCapExpansionAmount_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RevCapExpansionAmount = { "RevCapExpansionAmount", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, RevCapExpansionAmount), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RevCapExpansionAmount_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RevCapExpansionAmount_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RevCapFadeRingCount_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RevCapFadeRingCount = { "RevCapFadeRingCount", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, RevCapFadeRingCount), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RevCapFadeRingCount_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RevCapFadeRingCount_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingCount_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// number of inner rings to add to the fade ring\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "number of inner rings to add to the fade ring" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingCount = { "FadeRingCount", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, FadeRingCount), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingCount_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingCount_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFRScale_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// number of inner rings to add to the fade ring\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "number of inner rings to add to the fade ring" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFRScale = { "UV_RCFRScale", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, UV_RCFRScale), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFRScale_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFRScale_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFROffset_X_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// amount of scale we can ad to the UV assignment of the cap or dome\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "amount of scale we can ad to the UV assignment of the cap or dome" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFROffset_X = { "UV_RCFROffset_X", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, UV_RCFROffset_X), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFROffset_X_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFROffset_X_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFROffset_Y_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// amount of X offset we can assign to the UV in the cap or dome\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "amount of X offset we can assign to the UV in the cap or dome" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFROffset_Y = { "UV_RCFROffset_Y", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, UV_RCFROffset_Y), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFROffset_Y_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFROffset_Y_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFRRotation_MetaData[] = {
		{ "Category", "Limb Tools|EdgeloopInfo" },
		{ "Comment", "// amount of Y offset we can assign the UV in the cap or dome\n" },
		{ "ModuleRelativePath", "Public/FEdgeLoopInfo.h" },
		{ "ToolTip", "amount of Y offset we can assign the UV in the cap or dome" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFRRotation = { "UV_RCFRRotation", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfo, UV_RCFRRotation), METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFRRotation_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFRRotation_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VertexIndices_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VertexIndices,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_Positions_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_Positions,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingPositions_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingPositions,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UVs_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UVs,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_VotedMaterialIndex,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TotalEdgeLength,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddFadeRing,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddDome,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bAddNoise,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bFlatten,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_bUseManualNormal,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_LoopNumber,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_DomeHeight,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingHieght,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RingCount,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_NoiseAmount,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Rotation,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Scale,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Offset_X,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_Offset_Y,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_TwistFixOverride,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_PlanitaryThreshold,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_MaxNormalDeviation,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_SegmentSlopeTolerance,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_SegmentWindow,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_CapMaterial,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingMaterial,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_AssociatedBone,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_LimbEndCollapseAmount,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_ReverseCapFadeAmount,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RevCapExpansionAmount,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_RevCapFadeRingCount,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_FadeRingCount,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFRScale,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFROffset_X,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFROffset_Y,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewProp_UV_RCFRRotation,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_SkelMeshSliceTool,
		nullptr,
		&NewStructOps,
		"EdgeLoopInfo",
		sizeof(FEdgeLoopInfo),
		alignof(FEdgeLoopInfo),
		Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FEdgeLoopInfo()
	{
		if (!Z_Registration_Info_UScriptStruct_EdgeLoopInfo.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_EdgeLoopInfo.InnerSingleton, Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_EdgeLoopInfo.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FEdgeLoopInfo_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FEdgeLoopInfo_h_Statics::ScriptStructInfo[] = {
		{ FEdgeLoopInfo::StaticStruct, Z_Construct_UScriptStruct_FEdgeLoopInfo_Statics::NewStructOps, TEXT("EdgeLoopInfo"), &Z_Registration_Info_UScriptStruct_EdgeLoopInfo, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FEdgeLoopInfo), 797716121U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FEdgeLoopInfo_h_533304244(TEXT("/Script/SkelMeshSliceTool"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FEdgeLoopInfo_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FEdgeLoopInfo_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
