// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SkelMeshSliceTool/Public/ULimbExtractorTool.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeULimbExtractorTool() {}
// Cross Module References
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper();
	UPackage* Z_Construct_UPackage__Script_SkelMeshSliceTool();
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FEdgeLoopInfo();
	SKELMESHSLICETOOL_API UFunction* Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature();
	SKELMESHSLICETOOL_API UClass* Z_Construct_UClass_ULimbExtractorTool_NoRegister();
	SKELMESHSLICETOOL_API UClass* Z_Construct_UClass_ULimbExtractorTool();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FCapSkinningInfo();
	ENGINE_API UClass* Z_Construct_UClass_USkeletalMesh_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
	PROCEDURALMESHCOMPONENT_API UClass* Z_Construct_UClass_UProceduralMeshComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USkeletalMeshComponent_NoRegister();
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FRawVertexData();
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FBoneHierachyStruct();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector3f();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
	ENGINE_API UClass* Z_Construct_UClass_UMaterialInterface_NoRegister();
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FEdgeLoopCluster();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_EdgeLoopInfoArrayWrapper;
class UScriptStruct* FEdgeLoopInfoArrayWrapper::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_EdgeLoopInfoArrayWrapper.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_EdgeLoopInfoArrayWrapper.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper, Z_Construct_UPackage__Script_SkelMeshSliceTool(), TEXT("EdgeLoopInfoArrayWrapper"));
	}
	return Z_Registration_Info_UScriptStruct_EdgeLoopInfoArrayWrapper.OuterSingleton;
}
template<> SKELMESHSLICETOOL_API UScriptStruct* StaticStruct<FEdgeLoopInfoArrayWrapper>()
{
	return FEdgeLoopInfoArrayWrapper::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FStructPropertyParams NewProp_EdgeLoops_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EdgeLoops_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_EdgeLoops;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FEdgeLoopInfoArrayWrapper>();
	}
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::NewProp_EdgeLoops_Inner = { "EdgeLoops", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FEdgeLoopInfo, METADATA_PARAMS(nullptr, 0) }; // 797716121
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::NewProp_EdgeLoops_MetaData[] = {
		{ "Category", "Limb Tools|EdgeLoopInfoArrayWrapper" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::NewProp_EdgeLoops = { "EdgeLoops", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FEdgeLoopInfoArrayWrapper, EdgeLoops), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::NewProp_EdgeLoops_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::NewProp_EdgeLoops_MetaData)) }; // 797716121
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::NewProp_EdgeLoops_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::NewProp_EdgeLoops,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_SkelMeshSliceTool,
		nullptr,
		&NewStructOps,
		"EdgeLoopInfoArrayWrapper",
		sizeof(FEdgeLoopInfoArrayWrapper),
		alignof(FEdgeLoopInfoArrayWrapper),
		Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper()
	{
		if (!Z_Registration_Info_UScriptStruct_EdgeLoopInfoArrayWrapper.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_EdgeLoopInfoArrayWrapper.InnerSingleton, Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_EdgeLoopInfoArrayWrapper.InnerSingleton;
	}
	struct Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics
	{
		struct _Script_SkelMeshSliceTool_eventCapMenuPrompt_Parms
		{
			FEdgeLoopInfoArrayWrapper EdgeLoopInfoArray;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_EdgeLoopInfoArray;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics::NewProp_EdgeLoopInfoArray = { "EdgeLoopInfoArray", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_SkelMeshSliceTool_eventCapMenuPrompt_Parms, EdgeLoopInfoArray), Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper, METADATA_PARAMS(nullptr, 0) }; // 805799914
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics::NewProp_EdgeLoopInfoArray,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_SkelMeshSliceTool, nullptr, "CapMenuPrompt__DelegateSignature", nullptr, nullptr, sizeof(Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics::_Script_SkelMeshSliceTool_eventCapMenuPrompt_Parms), Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execGetOwnerActor)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(AActor**)Z_Param__Result=P_THIS->GetOwnerActor();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execLoggingAndDebugSwitches)
	{
		P_GET_UBOOL(Z_Param_ShowLoops);
		P_GET_UBOOL(Z_Param_MeshDiagnostic);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->LoggingAndDebugSwitches(Z_Param_ShowLoops,Z_Param_MeshDiagnostic);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execClearExcusionBoneList)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ClearExcusionBoneList();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execSetExclusionBones)
	{
		P_GET_TARRAY(FName,Z_Param_Exclusions);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SetExclusionBones(Z_Param_Exclusions);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execSetMinimumWeightThreshold)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_value);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SetMinimumWeightThreshold(Z_Param_value);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execPopulateSkelHierachy)
	{
		P_GET_OBJECT(USkeletalMesh,Z_Param_SkeletalMesh);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->PopulateSkelHierachy(Z_Param_SkeletalMesh);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execLogBoneIndices)
	{
		P_GET_OBJECT(USkeletalMesh,Z_Param_SkeletalMesh);
		P_GET_UBOOL(Z_Param_bAlsoLogSkeleton);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->LogBoneIndices(Z_Param_SkeletalMesh,Z_Param_bAlsoLogSkeleton);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execCreateFadeRingAndCap)
	{
		P_GET_STRUCT(FEdgeLoopInfo,Z_Param_LoopInfo);
		P_GET_STRUCT_REF(FEdgeLoopInfo,Z_Param_Out_UpdatedInfo);
		P_GET_STRUCT(FCapSkinningInfo,Z_Param_Info);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->CreateFadeRingAndCap(Z_Param_LoopInfo,Z_Param_Out_UpdatedInfo,Z_Param_Info);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execMergeCapsWithMesh)
	{
		P_GET_UBOOL(Z_Param_StaticMesh);
		P_GET_UBOOL(Z_Param_WeldMeshes);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->MergeCapsWithMesh(Z_Param_StaticMesh,Z_Param_WeldMeshes);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execCreateReverseCapPreview)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_index);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->CreateReverseCapPreview(Z_Param_index);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execSpawnRevCapActor)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_index);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SpawnRevCapActor(Z_Param_index);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execSpawnSkelMeshActor)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SpawnSkelMeshActor();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execSpawnProcMeshActor)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SpawnProcMeshActor();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execOnEditorUtilityClosed)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->OnEditorUtilityClosed();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ULimbExtractorTool::execExtractBoneToMesh)
	{
		P_GET_OBJECT(USkeletalMesh,Z_Param_SkeletalMesh);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_GET_UBOOL(Z_Param_CreateSkelMesh);
		P_GET_UBOOL(Z_Param_CreateBoneChain);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ExtractBoneToMesh(Z_Param_SkeletalMesh,Z_Param_BoneName,Z_Param_CreateSkelMesh,Z_Param_CreateBoneChain);
		P_NATIVE_END;
	}
	void ULimbExtractorTool::StaticRegisterNativesULimbExtractorTool()
	{
		UClass* Class = ULimbExtractorTool::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "ClearExcusionBoneList", &ULimbExtractorTool::execClearExcusionBoneList },
			{ "CreateFadeRingAndCap", &ULimbExtractorTool::execCreateFadeRingAndCap },
			{ "CreateReverseCapPreview", &ULimbExtractorTool::execCreateReverseCapPreview },
			{ "ExtractBoneToMesh", &ULimbExtractorTool::execExtractBoneToMesh },
			{ "GetOwnerActor", &ULimbExtractorTool::execGetOwnerActor },
			{ "LogBoneIndices", &ULimbExtractorTool::execLogBoneIndices },
			{ "LoggingAndDebugSwitches", &ULimbExtractorTool::execLoggingAndDebugSwitches },
			{ "MergeCapsWithMesh", &ULimbExtractorTool::execMergeCapsWithMesh },
			{ "OnEditorUtilityClosed", &ULimbExtractorTool::execOnEditorUtilityClosed },
			{ "PopulateSkelHierachy", &ULimbExtractorTool::execPopulateSkelHierachy },
			{ "SetExclusionBones", &ULimbExtractorTool::execSetExclusionBones },
			{ "SetMinimumWeightThreshold", &ULimbExtractorTool::execSetMinimumWeightThreshold },
			{ "SpawnProcMeshActor", &ULimbExtractorTool::execSpawnProcMeshActor },
			{ "SpawnRevCapActor", &ULimbExtractorTool::execSpawnRevCapActor },
			{ "SpawnSkelMeshActor", &ULimbExtractorTool::execSpawnSkelMeshActor },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_ClearExcusionBoneList_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_ClearExcusionBoneList_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Functions" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_ClearExcusionBoneList_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "ClearExcusionBoneList", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_ClearExcusionBoneList_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_ClearExcusionBoneList_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_ClearExcusionBoneList()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_ClearExcusionBoneList_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics
	{
		struct LimbExtractorTool_eventCreateFadeRingAndCap_Parms
		{
			FEdgeLoopInfo LoopInfo;
			FEdgeLoopInfo UpdatedInfo;
			FCapSkinningInfo Info;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_LoopInfo;
		static const UECodeGen_Private::FStructPropertyParams NewProp_UpdatedInfo;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Info;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::NewProp_LoopInfo = { "LoopInfo", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventCreateFadeRingAndCap_Parms, LoopInfo), Z_Construct_UScriptStruct_FEdgeLoopInfo, METADATA_PARAMS(nullptr, 0) }; // 797716121
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::NewProp_UpdatedInfo = { "UpdatedInfo", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventCreateFadeRingAndCap_Parms, UpdatedInfo), Z_Construct_UScriptStruct_FEdgeLoopInfo, METADATA_PARAMS(nullptr, 0) }; // 797716121
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::NewProp_Info = { "Info", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventCreateFadeRingAndCap_Parms, Info), Z_Construct_UScriptStruct_FCapSkinningInfo, METADATA_PARAMS(nullptr, 0) }; // 1885819128
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::NewProp_LoopInfo,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::NewProp_UpdatedInfo,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::NewProp_Info,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Functions" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "CreateFadeRingAndCap", nullptr, nullptr, sizeof(Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::LimbExtractorTool_eventCreateFadeRingAndCap_Parms), Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics
	{
		struct LimbExtractorTool_eventCreateReverseCapPreview_Parms
		{
			int32 index;
		};
		static const UECodeGen_Private::FIntPropertyParams NewProp_index;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics::NewProp_index = { "index", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventCreateReverseCapPreview_Parms, index), METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics::NewProp_index,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Functions" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "CreateReverseCapPreview", nullptr, nullptr, sizeof(Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics::LimbExtractorTool_eventCreateReverseCapPreview_Parms), Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics
	{
		struct LimbExtractorTool_eventExtractBoneToMesh_Parms
		{
			USkeletalMesh* SkeletalMesh;
			FName BoneName;
			bool CreateSkelMesh;
			bool CreateBoneChain;
		};
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SkeletalMesh;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static void NewProp_CreateSkelMesh_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_CreateSkelMesh;
		static void NewProp_CreateBoneChain_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_CreateBoneChain;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_SkeletalMesh = { "SkeletalMesh", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventExtractBoneToMesh_Parms, SkeletalMesh), Z_Construct_UClass_USkeletalMesh_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventExtractBoneToMesh_Parms, BoneName), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_CreateSkelMesh_SetBit(void* Obj)
	{
		((LimbExtractorTool_eventExtractBoneToMesh_Parms*)Obj)->CreateSkelMesh = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_CreateSkelMesh = { "CreateSkelMesh", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(LimbExtractorTool_eventExtractBoneToMesh_Parms), &Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_CreateSkelMesh_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_CreateBoneChain_SetBit(void* Obj)
	{
		((LimbExtractorTool_eventExtractBoneToMesh_Parms*)Obj)->CreateBoneChain = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_CreateBoneChain = { "CreateBoneChain", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(LimbExtractorTool_eventExtractBoneToMesh_Parms), &Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_CreateBoneChain_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_SkeletalMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_BoneName,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_CreateSkelMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::NewProp_CreateBoneChain,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Functions" },
		{ "Comment", "///////// Called by the editor utility and lets us run either the static mesh or skeletal mesh creation\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Called by the editor utility and lets us run either the static mesh or skeletal mesh creation" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "ExtractBoneToMesh", nullptr, nullptr, sizeof(Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::LimbExtractorTool_eventExtractBoneToMesh_Parms), Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics
	{
		struct LimbExtractorTool_eventGetOwnerActor_Parms
		{
			AActor* ReturnValue;
		};
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventGetOwnerActor_Parms, ReturnValue), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Functions" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "GetOwnerActor", nullptr, nullptr, sizeof(Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics::LimbExtractorTool_eventGetOwnerActor_Parms), Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics
	{
		struct LimbExtractorTool_eventLogBoneIndices_Parms
		{
			USkeletalMesh* SkeletalMesh;
			bool bAlsoLogSkeleton;
		};
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SkeletalMesh;
		static void NewProp_bAlsoLogSkeleton_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAlsoLogSkeleton;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::NewProp_SkeletalMesh = { "SkeletalMesh", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventLogBoneIndices_Parms, SkeletalMesh), Z_Construct_UClass_USkeletalMesh_NoRegister, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::NewProp_bAlsoLogSkeleton_SetBit(void* Obj)
	{
		((LimbExtractorTool_eventLogBoneIndices_Parms*)Obj)->bAlsoLogSkeleton = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::NewProp_bAlsoLogSkeleton = { "bAlsoLogSkeleton", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(LimbExtractorTool_eventLogBoneIndices_Parms), &Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::NewProp_bAlsoLogSkeleton_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::NewProp_SkeletalMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::NewProp_bAlsoLogSkeleton,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Logging" },
		{ "Comment", "/*= false*/" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "= false" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "LogBoneIndices", nullptr, nullptr, sizeof(Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::LimbExtractorTool_eventLogBoneIndices_Parms), Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics
	{
		struct LimbExtractorTool_eventLoggingAndDebugSwitches_Parms
		{
			bool ShowLoops;
			bool MeshDiagnostic;
		};
		static void NewProp_ShowLoops_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_ShowLoops;
		static void NewProp_MeshDiagnostic_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_MeshDiagnostic;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	void Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::NewProp_ShowLoops_SetBit(void* Obj)
	{
		((LimbExtractorTool_eventLoggingAndDebugSwitches_Parms*)Obj)->ShowLoops = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::NewProp_ShowLoops = { "ShowLoops", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(LimbExtractorTool_eventLoggingAndDebugSwitches_Parms), &Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::NewProp_ShowLoops_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::NewProp_MeshDiagnostic_SetBit(void* Obj)
	{
		((LimbExtractorTool_eventLoggingAndDebugSwitches_Parms*)Obj)->MeshDiagnostic = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::NewProp_MeshDiagnostic = { "MeshDiagnostic", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(LimbExtractorTool_eventLoggingAndDebugSwitches_Parms), &Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::NewProp_MeshDiagnostic_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::NewProp_ShowLoops,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::NewProp_MeshDiagnostic,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Functions" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "LoggingAndDebugSwitches", nullptr, nullptr, sizeof(Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::LimbExtractorTool_eventLoggingAndDebugSwitches_Parms), Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics
	{
		struct LimbExtractorTool_eventMergeCapsWithMesh_Parms
		{
			bool StaticMesh;
			bool WeldMeshes;
		};
		static void NewProp_StaticMesh_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_StaticMesh;
		static void NewProp_WeldMeshes_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_WeldMeshes;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	void Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::NewProp_StaticMesh_SetBit(void* Obj)
	{
		((LimbExtractorTool_eventMergeCapsWithMesh_Parms*)Obj)->StaticMesh = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::NewProp_StaticMesh = { "StaticMesh", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(LimbExtractorTool_eventMergeCapsWithMesh_Parms), &Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::NewProp_StaticMesh_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::NewProp_WeldMeshes_SetBit(void* Obj)
	{
		((LimbExtractorTool_eventMergeCapsWithMesh_Parms*)Obj)->WeldMeshes = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::NewProp_WeldMeshes = { "WeldMeshes", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(LimbExtractorTool_eventMergeCapsWithMesh_Parms), &Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::NewProp_WeldMeshes_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::NewProp_StaticMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::NewProp_WeldMeshes,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Functions" },
		{ "CPP_Default_StaticMesh", "false" },
		{ "CPP_Default_WeldMeshes", "true" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "MergeCapsWithMesh", nullptr, nullptr, sizeof(Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::LimbExtractorTool_eventMergeCapsWithMesh_Parms), Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_OnEditorUtilityClosed_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_OnEditorUtilityClosed_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Functions" },
		{ "Comment", "// when the editor utility is closed, it clears the static mesh, skeletal mesh and actor references. Deletes actor from the world\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "when the editor utility is closed, it clears the static mesh, skeletal mesh and actor references. Deletes actor from the world" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_OnEditorUtilityClosed_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "OnEditorUtilityClosed", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_OnEditorUtilityClosed_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_OnEditorUtilityClosed_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_OnEditorUtilityClosed()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_OnEditorUtilityClosed_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics
	{
		struct LimbExtractorTool_eventPopulateSkelHierachy_Parms
		{
			USkeletalMesh* SkeletalMesh;
		};
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SkeletalMesh;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics::NewProp_SkeletalMesh = { "SkeletalMesh", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventPopulateSkelHierachy_Parms, SkeletalMesh), Z_Construct_UClass_USkeletalMesh_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics::NewProp_SkeletalMesh,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Functions" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "PopulateSkelHierachy", nullptr, nullptr, sizeof(Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics::LimbExtractorTool_eventPopulateSkelHierachy_Parms), Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics
	{
		struct LimbExtractorTool_eventSetExclusionBones_Parms
		{
			TArray<FName> Exclusions;
		};
		static const UECodeGen_Private::FNamePropertyParams NewProp_Exclusions_Inner;
		static const UECodeGen_Private::FArrayPropertyParams NewProp_Exclusions;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::NewProp_Exclusions_Inner = { "Exclusions", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::NewProp_Exclusions = { "Exclusions", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventSetExclusionBones_Parms, Exclusions), EArrayPropertyFlags::None, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::NewProp_Exclusions_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::NewProp_Exclusions,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Functions" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "SetExclusionBones", nullptr, nullptr, sizeof(Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::LimbExtractorTool_eventSetExclusionBones_Parms), Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics
	{
		struct LimbExtractorTool_eventSetMinimumWeightThreshold_Parms
		{
			int32 value;
		};
		static const UECodeGen_Private::FIntPropertyParams NewProp_value;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics::NewProp_value = { "value", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventSetMinimumWeightThreshold_Parms, value), METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics::NewProp_value,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics::Function_MetaDataParams[] = {
		{ "Category", "Limb Tools|Functions" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "SetMinimumWeightThreshold", nullptr, nullptr, sizeof(Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics::LimbExtractorTool_eventSetMinimumWeightThreshold_Parms), Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_SpawnProcMeshActor_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_SpawnProcMeshActor_Statics::Function_MetaDataParams[] = {
		{ "Comment", "// spawns our preview actors with respective meshes attached\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "spawns our preview actors with respective meshes attached" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_SpawnProcMeshActor_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "SpawnProcMeshActor", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_SpawnProcMeshActor_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_SpawnProcMeshActor_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_SpawnProcMeshActor()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_SpawnProcMeshActor_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics
	{
		struct LimbExtractorTool_eventSpawnRevCapActor_Parms
		{
			int32 index;
		};
		static const UECodeGen_Private::FIntPropertyParams NewProp_index;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics::NewProp_index = { "index", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(LimbExtractorTool_eventSpawnRevCapActor_Parms, index), METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics::NewProp_index,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "SpawnRevCapActor", nullptr, nullptr, sizeof(Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics::LimbExtractorTool_eventSpawnRevCapActor_Parms), Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ULimbExtractorTool_SpawnSkelMeshActor_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ULimbExtractorTool_SpawnSkelMeshActor_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ULimbExtractorTool_SpawnSkelMeshActor_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ULimbExtractorTool, nullptr, "SpawnSkelMeshActor", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ULimbExtractorTool_SpawnSkelMeshActor_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ULimbExtractorTool_SpawnSkelMeshActor_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ULimbExtractorTool_SpawnSkelMeshActor()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ULimbExtractorTool_SpawnSkelMeshActor_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ULimbExtractorTool);
	UClass* Z_Construct_UClass_ULimbExtractorTool_NoRegister()
	{
		return ULimbExtractorTool::StaticClass();
	}
	struct Z_Construct_UClass_ULimbExtractorTool_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PreviewMesh_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_PreviewMesh;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OwnerActor_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OwnerActor;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OutLimbSkelMesh_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutLimbSkelMesh;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SkelMesh_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SkelMesh;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Wrapper_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_Wrapper;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InMesh_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_InMesh;
		static const UECodeGen_Private::FStructPropertyParams NewProp_RawVertexData_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RawVertexData_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_RawVertexData;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Prompt_MetaData[];
#endif
		static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_Prompt;
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ReverseCapComponents_ValueProp;
		static const UECodeGen_Private::FIntPropertyParams NewProp_ReverseCapComponents_Key_KeyProp;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReverseCapComponents_MetaData[];
#endif
		static const UECodeGen_Private::FMapPropertyParams NewProp_ReverseCapComponents;
		static const UECodeGen_Private::FNamePropertyParams NewProp_ExcludedBoneNames_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ExcludedBoneNames_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_ExcludedBoneNames;
		static const UECodeGen_Private::FStructPropertyParams NewProp_BoneHierachy_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BoneHierachy_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_BoneHierachy;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MinimumWeight_MetaData[];
#endif
		static const UECodeGen_Private::FBytePropertyParams NewProp_MinimumWeight;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ShowLoopDebug_MetaData[];
#endif
		static void NewProp_ShowLoopDebug_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_ShowLoopDebug;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DoDiagnostic_MetaData[];
#endif
		static void NewProp_DoDiagnostic_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_DoDiagnostic;
		static const UECodeGen_Private::FStructPropertyParams NewProp_BP_CapVertCache_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_CapVertCache_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_BP_CapVertCache;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_PlaneOrigin_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_BP_PlaneOrigin;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_PlaneNormal_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_BP_PlaneNormal;
		static const UECodeGen_Private::FStructPropertyParams NewProp_BP_OutUVs_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_OutUVs_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_BP_OutUVs;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_UVScale_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BP_UVScale;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_UVRotationDegrees_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BP_UVRotationDegrees;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_UVOffset_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_BP_UVOffset;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_RCFRUVScale_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BP_RCFRUVScale;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_RCFRUVRotationDegrees_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BP_RCFRUVRotationDegrees;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_RCFRUVOffset_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_BP_RCFRUVOffset;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_CapMaterial_MetaData[];
#endif
		static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_BP_CapMaterial;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_DomeHieght_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BP_DomeHieght;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_FadeRingLength_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BP_FadeRingLength;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_NoiseAmount_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BP_NoiseAmount;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_RingCount_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_BP_RingCount;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BP_AddNoise_MetaData[];
#endif
		static void NewProp_BP_AddNoise_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_BP_AddNoise;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AddDome_MetaData[];
#endif
		static void NewProp_AddDome_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_AddDome;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AddCap_MetaData[];
#endif
		static void NewProp_AddCap_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_AddCap;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EffectiveThreshold_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_EffectiveThreshold;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bEnableLoopClustering_MetaData[];
#endif
		static void NewProp_bEnableLoopClustering_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableLoopClustering;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClusterRadius_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ClusterRadius;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MinLoopArea_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MinLoopArea;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bMergeOverlappingLoops_MetaData[];
#endif
		static void NewProp_bMergeOverlappingLoops_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bMergeOverlappingLoops;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bFilterSmallFragments_MetaData[];
#endif
		static void NewProp_bFilterSmallFragments_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bFilterSmallFragments;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxMergeDistance_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxMergeDistance;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bPreserveAllVertexIndices_MetaData[];
#endif
		static void NewProp_bPreserveAllVertexIndices_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bPreserveAllVertexIndices;
		static const UECodeGen_Private::FStructPropertyParams NewProp_DetectedClusters_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DetectedClusters_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_DetectedClusters;
		static const UECodeGen_Private::FStructPropertyParams NewProp_ClusteredEdgeLoopInfos_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClusteredEdgeLoopInfos_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_ClusteredEdgeLoopInfos;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ULimbExtractorTool_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_SkelMeshSliceTool,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_ULimbExtractorTool_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_ULimbExtractorTool_ClearExcusionBoneList, "ClearExcusionBoneList" }, // 308178494
		{ &Z_Construct_UFunction_ULimbExtractorTool_CreateFadeRingAndCap, "CreateFadeRingAndCap" }, // 1661172018
		{ &Z_Construct_UFunction_ULimbExtractorTool_CreateReverseCapPreview, "CreateReverseCapPreview" }, // 893292381
		{ &Z_Construct_UFunction_ULimbExtractorTool_ExtractBoneToMesh, "ExtractBoneToMesh" }, // 3266765144
		{ &Z_Construct_UFunction_ULimbExtractorTool_GetOwnerActor, "GetOwnerActor" }, // 1970996413
		{ &Z_Construct_UFunction_ULimbExtractorTool_LogBoneIndices, "LogBoneIndices" }, // 783333034
		{ &Z_Construct_UFunction_ULimbExtractorTool_LoggingAndDebugSwitches, "LoggingAndDebugSwitches" }, // 7861767
		{ &Z_Construct_UFunction_ULimbExtractorTool_MergeCapsWithMesh, "MergeCapsWithMesh" }, // 3628940182
		{ &Z_Construct_UFunction_ULimbExtractorTool_OnEditorUtilityClosed, "OnEditorUtilityClosed" }, // 2481009602
		{ &Z_Construct_UFunction_ULimbExtractorTool_PopulateSkelHierachy, "PopulateSkelHierachy" }, // 4198287652
		{ &Z_Construct_UFunction_ULimbExtractorTool_SetExclusionBones, "SetExclusionBones" }, // 909582721
		{ &Z_Construct_UFunction_ULimbExtractorTool_SetMinimumWeightThreshold, "SetMinimumWeightThreshold" }, // 2033135652
		{ &Z_Construct_UFunction_ULimbExtractorTool_SpawnProcMeshActor, "SpawnProcMeshActor" }, // 3382904631
		{ &Z_Construct_UFunction_ULimbExtractorTool_SpawnRevCapActor, "SpawnRevCapActor" }, // 1835911641
		{ &Z_Construct_UFunction_ULimbExtractorTool_SpawnSkelMeshActor, "SpawnSkelMeshActor" }, // 3537992598
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "//FORCEINLINE uint32 GetTypeHash(const FVertexKey& Key)\n//{\n//    uint32 Hash = HashCombine(::GetTypeHash(Key.Position), ::GetTypeHash(Key.UV));\n//    Hash = HashCombine(Hash, ::GetTypeHash(Key.Normal));\n//    return Hash;\n//}\n" },
		{ "IncludePath", "ULimbExtractorTool.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "FORCEINLINE uint32 GetTypeHash(const FVertexKey& Key)\n{\n    uint32 Hash = HashCombine(::GetTypeHash(Key.Position), ::GetTypeHash(Key.UV));\n    Hash = HashCombine(Hash, ::GetTypeHash(Key.Normal));\n    return Hash;\n}" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_PreviewMesh_MetaData[] = {
		{ "Comment", "///// The main use members /////////////////\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "The main use members" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_PreviewMesh = { "PreviewMesh", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, PreviewMesh), Z_Construct_UClass_UProceduralMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_PreviewMesh_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_PreviewMesh_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_OwnerActor_MetaData[] = {
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_OwnerActor = { "OwnerActor", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, OwnerActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_OwnerActor_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_OwnerActor_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_OutLimbSkelMesh_MetaData[] = {
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_OutLimbSkelMesh = { "OutLimbSkelMesh", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, OutLimbSkelMesh), Z_Construct_UClass_USkeletalMesh_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_OutLimbSkelMesh_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_OutLimbSkelMesh_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_SkelMesh_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_SkelMesh = { "SkelMesh", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, SkelMesh), Z_Construct_UClass_USkeletalMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_SkelMesh_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_SkelMesh_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_Wrapper_MetaData[] = {
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_Wrapper = { "Wrapper", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, Wrapper), Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_Wrapper_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_Wrapper_MetaData)) }; // 805799914
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_InMesh_MetaData[] = {
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_InMesh = { "InMesh", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, InMesh), Z_Construct_UClass_USkeletalMesh_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_InMesh_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_InMesh_MetaData)) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_RawVertexData_Inner = { "RawVertexData", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FRawVertexData, METADATA_PARAMS(nullptr, 0) }; // 750934896
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_RawVertexData_MetaData[] = {
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_RawVertexData = { "RawVertexData", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, RawVertexData), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_RawVertexData_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_RawVertexData_MetaData)) }; // 750934896
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_Prompt_MetaData[] = {
		{ "Category", "Limb Tools|Callbacks" },
		{ "Comment", "///// To broadcast to the menu ////////\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "To broadcast to the menu" },
	};
#endif
	const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_Prompt = { "Prompt", nullptr, (EPropertyFlags)0x0040000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, Prompt), Z_Construct_UDelegateFunction_SkelMeshSliceTool_CapMenuPrompt__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_Prompt_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_Prompt_MetaData)) }; // 3455004082
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ReverseCapComponents_ValueProp = { "ReverseCapComponents", nullptr, (EPropertyFlags)0x00000000000a0009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UClass_UProceduralMeshComponent_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ReverseCapComponents_Key_KeyProp = { "ReverseCapComponents_Key", nullptr, (EPropertyFlags)0x00000000000a0009, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ReverseCapComponents_MetaData[] = {
		{ "Category", "Limb Tools|ReverseCaps" },
		{ "Comment", "// This is to hold our map of reverse caps to match the generated caps - i:e - wounds to attach to the character mesh\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "This is to hold our map of reverse caps to match the generated caps - i:e - wounds to attach to the character mesh" },
	};
#endif
	const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ReverseCapComponents = { "ReverseCapComponents", nullptr, (EPropertyFlags)0x002008800002001d, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, ReverseCapComponents), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ReverseCapComponents_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ReverseCapComponents_MetaData)) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ExcludedBoneNames_Inner = { "ExcludedBoneNames", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ExcludedBoneNames_MetaData[] = {
		{ "Category", "Limb Tools|Variables" },
		{ "Comment", "////////  Exposed members for use in the menu //////////////////\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Exposed members for use in the menu" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ExcludedBoneNames = { "ExcludedBoneNames", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, ExcludedBoneNames), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ExcludedBoneNames_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ExcludedBoneNames_MetaData)) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BoneHierachy_Inner = { "BoneHierachy", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FBoneHierachyStruct, METADATA_PARAMS(nullptr, 0) }; // 3251922161
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BoneHierachy_MetaData[] = {
		{ "Category", "Limb Tools|Variables" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BoneHierachy = { "BoneHierachy", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BoneHierachy), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BoneHierachy_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BoneHierachy_MetaData)) }; // 3251922161
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MinimumWeight_MetaData[] = {
		{ "Category", "Limb Tools|Variables" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MinimumWeight = { "MinimumWeight", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, MinimumWeight), nullptr, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MinimumWeight_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MinimumWeight_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ShowLoopDebug_MetaData[] = {
		{ "Comment", "////////  Set using the public function for debug //////////////////\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Set using the public function for debug" },
	};
#endif
	void Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ShowLoopDebug_SetBit(void* Obj)
	{
		((ULimbExtractorTool*)Obj)->ShowLoopDebug = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ShowLoopDebug = { "ShowLoopDebug", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ULimbExtractorTool), &Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ShowLoopDebug_SetBit, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ShowLoopDebug_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ShowLoopDebug_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DoDiagnostic_MetaData[] = {
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	void Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DoDiagnostic_SetBit(void* Obj)
	{
		((ULimbExtractorTool*)Obj)->DoDiagnostic = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DoDiagnostic = { "DoDiagnostic", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ULimbExtractorTool), &Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DoDiagnostic_SetBit, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DoDiagnostic_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DoDiagnostic_MetaData)) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapVertCache_Inner = { "BP_CapVertCache", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FVector3f, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapVertCache_MetaData[] = {
		{ "Category", "Limb Tools|UV Controls" },
		{ "Comment", "/////// Exposed members for the editor tool to control UV layout on the caps  ////////////////////\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "/ Exposed members for the editor tool to control UV layout on the caps" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapVertCache = { "BP_CapVertCache", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_CapVertCache), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapVertCache_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapVertCache_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_PlaneOrigin_MetaData[] = {
		{ "Category", "Limb Tools|UV Controls" },
		{ "Comment", "//Cap triangle vertex positions(flat)\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Cap triangle vertex positions(flat)" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_PlaneOrigin = { "BP_PlaneOrigin", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_PlaneOrigin), Z_Construct_UScriptStruct_FVector3f, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_PlaneOrigin_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_PlaneOrigin_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_PlaneNormal_MetaData[] = {
		{ "Category", "Limb Tools|UV Controls" },
		{ "Comment", "//A point on the slicing plane(e.g.loop centroid)\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "A point on the slicing plane(e.g.loop centroid)" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_PlaneNormal = { "BP_PlaneNormal", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_PlaneNormal), Z_Construct_UScriptStruct_FVector3f, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_PlaneNormal_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_PlaneNormal_MetaData)) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_OutUVs_Inner = { "BP_OutUVs", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_OutUVs_MetaData[] = {
		{ "Category", "Limb Tools|UV Controls" },
		{ "Comment", "//Normal of the slicing plane(facing out from limb)\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Normal of the slicing plane(facing out from limb)" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_OutUVs = { "BP_OutUVs", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_OutUVs), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_OutUVs_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_OutUVs_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVScale_MetaData[] = {
		{ "Category", "Limb Tools|UV Controls" },
		{ "Comment", "//Output UVs(1:1 with CapVertices)\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Output UVs(1:1 with CapVertices)" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVScale = { "BP_UVScale", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_UVScale), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVScale_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVScale_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVRotationDegrees_MetaData[] = {
		{ "Category", "Limb Tools|UV Controls" },
		{ "Comment", "//Controls tiling(default 1.0f)\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Controls tiling(default 1.0f)" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVRotationDegrees = { "BP_UVRotationDegrees", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_UVRotationDegrees), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVRotationDegrees_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVRotationDegrees_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVOffset_MetaData[] = {
		{ "Category", "Limb Tools|UV Controls" },
		{ "Comment", "//Degrees to rotate UVs around center\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Degrees to rotate UVs around center" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVOffset = { "BP_UVOffset", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_UVOffset), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVOffset_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVOffset_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVScale_MetaData[] = {
		{ "Category", "Limb Tools|UV Controls" },
		{ "Comment", "////// Reverse Cap FadeRing UV controls\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Reverse Cap FadeRing UV controls" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVScale = { "BP_RCFRUVScale", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_RCFRUVScale), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVScale_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVScale_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVRotationDegrees_MetaData[] = {
		{ "Category", "Limb Tools|UV Controls" },
		{ "Comment", "//Controls tiling(default 1.0f)\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Controls tiling(default 1.0f)" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVRotationDegrees = { "BP_RCFRUVRotationDegrees", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_RCFRUVRotationDegrees), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVRotationDegrees_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVRotationDegrees_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVOffset_MetaData[] = {
		{ "Category", "Limb Tools|UV Controls" },
		{ "Comment", "//Degrees to rotate UVs around center\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Degrees to rotate UVs around center" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVOffset = { "BP_RCFRUVOffset", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_RCFRUVOffset), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVOffset_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVOffset_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapMaterial_MetaData[] = {
		{ "Category", "Limb Tools|UV Texture" },
		{ "Comment", "///////////////// Exposed members to control cap doming, material, and noise /////////////////////\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Exposed members to control cap doming, material, and noise" },
	};
#endif
	const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapMaterial = { "BP_CapMaterial", nullptr, (EPropertyFlags)0x0014000000000004, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_CapMaterial), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapMaterial_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapMaterial_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_DomeHieght_MetaData[] = {
		{ "Category", "Limb Tools|Cap structure controls" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_DomeHieght = { "BP_DomeHieght", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_DomeHieght), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_DomeHieght_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_DomeHieght_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_FadeRingLength_MetaData[] = {
		{ "Category", "Limb Tools|Cap structure controls" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_FadeRingLength = { "BP_FadeRingLength", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_FadeRingLength), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_FadeRingLength_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_FadeRingLength_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_NoiseAmount_MetaData[] = {
		{ "Category", "Limb Tools|Cap structure controls" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_NoiseAmount = { "BP_NoiseAmount", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_NoiseAmount), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_NoiseAmount_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_NoiseAmount_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RingCount_MetaData[] = {
		{ "Category", "Limb Tools|Cap structure controls" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RingCount = { "BP_RingCount", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, BP_RingCount), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RingCount_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RingCount_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_AddNoise_MetaData[] = {
		{ "Category", "Limb Tools|Cap structure controls" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	void Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_AddNoise_SetBit(void* Obj)
	{
		((ULimbExtractorTool*)Obj)->BP_AddNoise = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_AddNoise = { "BP_AddNoise", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ULimbExtractorTool), &Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_AddNoise_SetBit, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_AddNoise_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_AddNoise_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddDome_MetaData[] = {
		{ "Category", "Limb Tools|Cap structure controls" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	void Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddDome_SetBit(void* Obj)
	{
		((ULimbExtractorTool*)Obj)->AddDome = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddDome = { "AddDome", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ULimbExtractorTool), &Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddDome_SetBit, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddDome_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddDome_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddCap_MetaData[] = {
		{ "Category", "Limb Tools|Cap structure controls" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	void Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddCap_SetBit(void* Obj)
	{
		((ULimbExtractorTool*)Obj)->AddCap = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddCap = { "AddCap", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ULimbExtractorTool), &Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddCap_SetBit, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddCap_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddCap_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_EffectiveThreshold_MetaData[] = {
		{ "Category", "Limb Tools|Cap structure controls" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_EffectiveThreshold = { "EffectiveThreshold", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, EffectiveThreshold), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_EffectiveThreshold_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_EffectiveThreshold_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bEnableLoopClustering_MetaData[] = {
		{ "Category", "Edge Loop Clustering" },
		{ "Comment", "/////////////////////////////////////////////////////////////////////////////////////////////////////\n// New clustering configuration properties\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "New clustering configuration properties" },
	};
#endif
	void Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bEnableLoopClustering_SetBit(void* Obj)
	{
		((ULimbExtractorTool*)Obj)->bEnableLoopClustering = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bEnableLoopClustering = { "bEnableLoopClustering", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ULimbExtractorTool), &Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bEnableLoopClustering_SetBit, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bEnableLoopClustering_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bEnableLoopClustering_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusterRadius_MetaData[] = {
		{ "Category", "Edge Loop Clustering" },
		{ "EditCondition", "bEnableLoopClustering" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusterRadius = { "ClusterRadius", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, ClusterRadius), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusterRadius_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusterRadius_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MinLoopArea_MetaData[] = {
		{ "Category", "Edge Loop Clustering" },
		{ "EditCondition", "bEnableLoopClustering" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MinLoopArea = { "MinLoopArea", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, MinLoopArea), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MinLoopArea_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MinLoopArea_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bMergeOverlappingLoops_MetaData[] = {
		{ "Category", "Edge Loop Clustering" },
		{ "EditCondition", "bEnableLoopClustering" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	void Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bMergeOverlappingLoops_SetBit(void* Obj)
	{
		((ULimbExtractorTool*)Obj)->bMergeOverlappingLoops = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bMergeOverlappingLoops = { "bMergeOverlappingLoops", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ULimbExtractorTool), &Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bMergeOverlappingLoops_SetBit, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bMergeOverlappingLoops_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bMergeOverlappingLoops_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bFilterSmallFragments_MetaData[] = {
		{ "Category", "Edge Loop Clustering" },
		{ "Comment", "// DISABLED BY DEFAULT to preserve data\n" },
		{ "EditCondition", "bEnableLoopClustering" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "DISABLED BY DEFAULT to preserve data" },
	};
#endif
	void Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bFilterSmallFragments_SetBit(void* Obj)
	{
		((ULimbExtractorTool*)Obj)->bFilterSmallFragments = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bFilterSmallFragments = { "bFilterSmallFragments", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ULimbExtractorTool), &Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bFilterSmallFragments_SetBit, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bFilterSmallFragments_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bFilterSmallFragments_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MaxMergeDistance_MetaData[] = {
		{ "Category", "Edge Loop Clustering" },
		{ "Comment", "// New safety parameters\n" },
		{ "EditCondition", "bEnableLoopClustering" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "New safety parameters" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MaxMergeDistance = { "MaxMergeDistance", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, MaxMergeDistance), METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MaxMergeDistance_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MaxMergeDistance_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bPreserveAllVertexIndices_MetaData[] = {
		{ "Category", "Edge Loop Clustering" },
		{ "Comment", "// Only merge loops within this distance\n" },
		{ "EditCondition", "bEnableLoopClustering" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Only merge loops within this distance" },
	};
#endif
	void Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bPreserveAllVertexIndices_SetBit(void* Obj)
	{
		((ULimbExtractorTool*)Obj)->bPreserveAllVertexIndices = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bPreserveAllVertexIndices = { "bPreserveAllVertexIndices", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ULimbExtractorTool), &Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bPreserveAllVertexIndices_SetBit, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bPreserveAllVertexIndices_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bPreserveAllVertexIndices_MetaData)) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DetectedClusters_Inner = { "DetectedClusters", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FEdgeLoopCluster, METADATA_PARAMS(nullptr, 0) }; // 1779035018
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DetectedClusters_MetaData[] = {
		{ "Category", "Edge Loop Results" },
		{ "Comment", "// Enhanced data structures\n" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
		{ "ToolTip", "Enhanced data structures" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DetectedClusters = { "DetectedClusters", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, DetectedClusters), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DetectedClusters_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DetectedClusters_MetaData)) }; // 1779035018
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusteredEdgeLoopInfos_Inner = { "ClusteredEdgeLoopInfos", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FEdgeLoopInfo, METADATA_PARAMS(nullptr, 0) }; // 797716121
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusteredEdgeLoopInfos_MetaData[] = {
		{ "Category", "Edge Loop Results" },
		{ "ModuleRelativePath", "Public/ULimbExtractorTool.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusteredEdgeLoopInfos = { "ClusteredEdgeLoopInfos", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULimbExtractorTool, ClusteredEdgeLoopInfos), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusteredEdgeLoopInfos_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusteredEdgeLoopInfos_MetaData)) }; // 797716121
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ULimbExtractorTool_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_PreviewMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_OwnerActor,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_OutLimbSkelMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_SkelMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_Wrapper,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_InMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_RawVertexData_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_RawVertexData,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_Prompt,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ReverseCapComponents_ValueProp,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ReverseCapComponents_Key_KeyProp,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ReverseCapComponents,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ExcludedBoneNames_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ExcludedBoneNames,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BoneHierachy_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BoneHierachy,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MinimumWeight,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ShowLoopDebug,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DoDiagnostic,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapVertCache_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapVertCache,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_PlaneOrigin,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_PlaneNormal,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_OutUVs_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_OutUVs,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVScale,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVRotationDegrees,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_UVOffset,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVScale,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVRotationDegrees,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RCFRUVOffset,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_CapMaterial,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_DomeHieght,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_FadeRingLength,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_NoiseAmount,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_RingCount,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_BP_AddNoise,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddDome,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_AddCap,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_EffectiveThreshold,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bEnableLoopClustering,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusterRadius,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MinLoopArea,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bMergeOverlappingLoops,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bFilterSmallFragments,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_MaxMergeDistance,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_bPreserveAllVertexIndices,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DetectedClusters_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_DetectedClusters,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusteredEdgeLoopInfos_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULimbExtractorTool_Statics::NewProp_ClusteredEdgeLoopInfos,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ULimbExtractorTool_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ULimbExtractorTool>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ULimbExtractorTool_Statics::ClassParams = {
		&ULimbExtractorTool::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_ULimbExtractorTool_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::PropPointers),
		0,
		0x008000A0u,
		METADATA_PARAMS(Z_Construct_UClass_ULimbExtractorTool_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ULimbExtractorTool_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ULimbExtractorTool()
	{
		if (!Z_Registration_Info_UClass_ULimbExtractorTool.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ULimbExtractorTool.OuterSingleton, Z_Construct_UClass_ULimbExtractorTool_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ULimbExtractorTool.OuterSingleton;
	}
	template<> SKELMESHSLICETOOL_API UClass* StaticClass<ULimbExtractorTool>()
	{
		return ULimbExtractorTool::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ULimbExtractorTool);
	struct Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_Statics::ScriptStructInfo[] = {
		{ FEdgeLoopInfoArrayWrapper::StaticStruct, Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics::NewStructOps, TEXT("EdgeLoopInfoArrayWrapper"), &Z_Registration_Info_UScriptStruct_EdgeLoopInfoArrayWrapper, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FEdgeLoopInfoArrayWrapper), 805799914U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ULimbExtractorTool, ULimbExtractorTool::StaticClass, TEXT("ULimbExtractorTool"), &Z_Registration_Info_UClass_ULimbExtractorTool, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ULimbExtractorTool), 3550964873U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_3643531766(TEXT("/Script/SkelMeshSliceTool"),
		Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
