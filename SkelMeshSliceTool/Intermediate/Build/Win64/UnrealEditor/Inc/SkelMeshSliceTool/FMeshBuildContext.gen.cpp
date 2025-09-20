// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SkelMeshSliceTool/Public/FMeshBuildContext.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFMeshBuildContext() {}
// Cross Module References
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FMeshBuildContext();
	UPackage* Z_Construct_UPackage__Script_SkelMeshSliceTool();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_MeshBuildContext;
class UScriptStruct* FMeshBuildContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_MeshBuildContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_MeshBuildContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMeshBuildContext, Z_Construct_UPackage__Script_SkelMeshSliceTool(), TEXT("MeshBuildContext"));
	}
	return Z_Registration_Info_UScriptStruct_MeshBuildContext.OuterSingleton;
}
template<> SKELMESHSLICETOOL_API UScriptStruct* StaticStruct<FMeshBuildContext>()
{
	return FMeshBuildContext::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FMeshBuildContext_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FMeshBuildContext_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/FMeshBuildContext.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FMeshBuildContext_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMeshBuildContext>();
	}
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMeshBuildContext_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_SkelMeshSliceTool,
		nullptr,
		&NewStructOps,
		"MeshBuildContext",
		sizeof(FMeshBuildContext),
		alignof(FMeshBuildContext),
		nullptr,
		0,
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FMeshBuildContext_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMeshBuildContext_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FMeshBuildContext()
	{
		if (!Z_Registration_Info_UScriptStruct_MeshBuildContext.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_MeshBuildContext.InnerSingleton, Z_Construct_UScriptStruct_FMeshBuildContext_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_MeshBuildContext.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FMeshBuildContext_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FMeshBuildContext_h_Statics::ScriptStructInfo[] = {
		{ FMeshBuildContext::StaticStruct, Z_Construct_UScriptStruct_FMeshBuildContext_Statics::NewStructOps, TEXT("MeshBuildContext"), &Z_Registration_Info_UScriptStruct_MeshBuildContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMeshBuildContext), 3425915680U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FMeshBuildContext_h_2463598448(TEXT("/Script/SkelMeshSliceTool"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FMeshBuildContext_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_FMeshBuildContext_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
