// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SkelMeshSliceTool/Public/FCapSkinningInfo.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFCapSkinningInfo() {}
// Cross Module References
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FCapSkinningInfo();
	UPackage* Z_Construct_UPackage__Script_SkelMeshSliceTool();
	SKELMESHSLICETOOL_API UEnum* Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_CapSkinningInfo;
class UScriptStruct* FCapSkinningInfo::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_CapSkinningInfo.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_CapSkinningInfo.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FCapSkinningInfo, Z_Construct_UPackage__Script_SkelMeshSliceTool(), TEXT("CapSkinningInfo"));
	}
	return Z_Registration_Info_UScriptStruct_CapSkinningInfo.OuterSingleton;
}
template<> SKELMESHSLICETOOL_API UScriptStruct* StaticStruct<FCapSkinningInfo>()
{
	return FCapSkinningInfo::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FCapSkinningInfo_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FBytePropertyParams NewProp_SkinningMethod_Underlying;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SkinningMethod_MetaData[];
#endif
		static const UECodeGen_Private::FEnumPropertyParams NewProp_SkinningMethod;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ProximityRadius_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ProximityRadius;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/FCapSkinningInfo.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FCapSkinningInfo>();
	}
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_SkinningMethod_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_SkinningMethod_MetaData[] = {
		{ "Category", "Limb Tools|CapSkinningInfo" },
		{ "Comment", "// set in code during the Mesh bilding\n" },
		{ "ModuleRelativePath", "Public/FCapSkinningInfo.h" },
		{ "ToolTip", "set in code during the Mesh bilding" },
	};
#endif
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_SkinningMethod = { "SkinningMethod", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FCapSkinningInfo, SkinningMethod), Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType, METADATA_PARAMS(Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_SkinningMethod_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_SkinningMethod_MetaData)) }; // 4077510313
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_ProximityRadius_MetaData[] = {
		{ "Category", "Limb Tools|CapSkinningInfo" },
		{ "Comment", "// choose between Rigid binding and Proximity based binding\n" },
		{ "ModuleRelativePath", "Public/FCapSkinningInfo.h" },
		{ "ToolTip", "choose between Rigid binding and Proximity based binding" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_ProximityRadius = { "ProximityRadius", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FCapSkinningInfo, ProximityRadius), METADATA_PARAMS(Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_ProximityRadius_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_ProximityRadius_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_SkinningMethod_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_SkinningMethod,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewProp_ProximityRadius,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_SkelMeshSliceTool,
		nullptr,
		&NewStructOps,
		"CapSkinningInfo",
		sizeof(FCapSkinningInfo),
		alignof(FCapSkinningInfo),
		Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FCapSkinningInfo()
	{
		if (!Z_Registration_Info_UScriptStruct_CapSkinningInfo.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_CapSkinningInfo.InnerSingleton, Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_CapSkinningInfo.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FCapSkinningInfo_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FCapSkinningInfo_h_Statics::ScriptStructInfo[] = {
		{ FCapSkinningInfo::StaticStruct, Z_Construct_UScriptStruct_FCapSkinningInfo_Statics::NewStructOps, TEXT("CapSkinningInfo"), &Z_Registration_Info_UScriptStruct_CapSkinningInfo, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FCapSkinningInfo), 1885819128U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FCapSkinningInfo_h_1656762500(TEXT("/Script/SkelMeshSliceTool"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FCapSkinningInfo_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FCapSkinningInfo_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
