// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SkelMeshSliceTool/Public/FRawVertexData.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFRawVertexData() {}
// Cross Module References
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FRawVertexData();
	UPackage* Z_Construct_UPackage__Script_SkelMeshSliceTool();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_RawVertexData;
class UScriptStruct* FRawVertexData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_RawVertexData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_RawVertexData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FRawVertexData, Z_Construct_UPackage__Script_SkelMeshSliceTool(), TEXT("RawVertexData"));
	}
	return Z_Registration_Info_UScriptStruct_RawVertexData.OuterSingleton;
}
template<> SKELMESHSLICETOOL_API UScriptStruct* StaticStruct<FRawVertexData>()
{
	return FRawVertexData::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FRawVertexData_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FRawVertexData_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/FRawVertexData.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FRawVertexData_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FRawVertexData>();
	}
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FRawVertexData_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_SkelMeshSliceTool,
		nullptr,
		&NewStructOps,
		"RawVertexData",
		sizeof(FRawVertexData),
		alignof(FRawVertexData),
		nullptr,
		0,
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FRawVertexData_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FRawVertexData_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FRawVertexData()
	{
		if (!Z_Registration_Info_UScriptStruct_RawVertexData.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_RawVertexData.InnerSingleton, Z_Construct_UScriptStruct_FRawVertexData_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_RawVertexData.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FRawVertexData_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FRawVertexData_h_Statics::ScriptStructInfo[] = {
		{ FRawVertexData::StaticStruct, Z_Construct_UScriptStruct_FRawVertexData_Statics::NewStructOps, TEXT("RawVertexData"), &Z_Registration_Info_UScriptStruct_RawVertexData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FRawVertexData), 750934896U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FRawVertexData_h_3330775546(TEXT("/Script/SkelMeshSliceTool"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FRawVertexData_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FRawVertexData_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
