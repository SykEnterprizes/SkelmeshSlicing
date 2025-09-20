// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SkelMeshSliceTool/Public/UEdgeLoopClusteringUtility.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUEdgeLoopClusteringUtility() {}
// Cross Module References
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FEdgeLoopCluster();
	UPackage* Z_Construct_UPackage__Script_SkelMeshSliceTool();
	SKELMESHSLICETOOL_API UClass* Z_Construct_UClass_UEdgeLoopClusteringUtility_NoRegister();
	SKELMESHSLICETOOL_API UClass* Z_Construct_UClass_UEdgeLoopClusteringUtility();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_EdgeLoopCluster;
class UScriptStruct* FEdgeLoopCluster::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_EdgeLoopCluster.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_EdgeLoopCluster.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FEdgeLoopCluster, Z_Construct_UPackage__Script_SkelMeshSliceTool(), TEXT("EdgeLoopCluster"));
	}
	return Z_Registration_Info_UScriptStruct_EdgeLoopCluster.OuterSingleton;
}
template<> SKELMESHSLICETOOL_API UScriptStruct* StaticStruct<FEdgeLoopCluster>()
{
	return FEdgeLoopCluster::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FEdgeLoopCluster_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FEdgeLoopCluster_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/UEdgeLoopClusteringUtility.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FEdgeLoopCluster_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FEdgeLoopCluster>();
	}
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FEdgeLoopCluster_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_SkelMeshSliceTool,
		nullptr,
		&NewStructOps,
		"EdgeLoopCluster",
		sizeof(FEdgeLoopCluster),
		alignof(FEdgeLoopCluster),
		nullptr,
		0,
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FEdgeLoopCluster_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEdgeLoopCluster_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FEdgeLoopCluster()
	{
		if (!Z_Registration_Info_UScriptStruct_EdgeLoopCluster.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_EdgeLoopCluster.InnerSingleton, Z_Construct_UScriptStruct_FEdgeLoopCluster_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_EdgeLoopCluster.InnerSingleton;
	}
	void UEdgeLoopClusteringUtility::StaticRegisterNativesUEdgeLoopClusteringUtility()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UEdgeLoopClusteringUtility);
	UClass* Z_Construct_UClass_UEdgeLoopClusteringUtility_NoRegister()
	{
		return UEdgeLoopClusteringUtility::StaticClass();
	}
	struct Z_Construct_UClass_UEdgeLoopClusteringUtility_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UEdgeLoopClusteringUtility_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_SkelMeshSliceTool,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEdgeLoopClusteringUtility_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "UEdgeLoopClusteringUtility.h" },
		{ "ModuleRelativePath", "Public/UEdgeLoopClusteringUtility.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UEdgeLoopClusteringUtility_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UEdgeLoopClusteringUtility>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UEdgeLoopClusteringUtility_Statics::ClassParams = {
		&UEdgeLoopClusteringUtility::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UEdgeLoopClusteringUtility_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UEdgeLoopClusteringUtility_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UEdgeLoopClusteringUtility()
	{
		if (!Z_Registration_Info_UClass_UEdgeLoopClusteringUtility.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UEdgeLoopClusteringUtility.OuterSingleton, Z_Construct_UClass_UEdgeLoopClusteringUtility_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UEdgeLoopClusteringUtility.OuterSingleton;
	}
	template<> SKELMESHSLICETOOL_API UClass* StaticClass<UEdgeLoopClusteringUtility>()
	{
		return UEdgeLoopClusteringUtility::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UEdgeLoopClusteringUtility);
	struct Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UEdgeLoopClusteringUtility_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UEdgeLoopClusteringUtility_h_Statics::ScriptStructInfo[] = {
		{ FEdgeLoopCluster::StaticStruct, Z_Construct_UScriptStruct_FEdgeLoopCluster_Statics::NewStructOps, TEXT("EdgeLoopCluster"), &Z_Registration_Info_UScriptStruct_EdgeLoopCluster, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FEdgeLoopCluster), 1779035018U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UEdgeLoopClusteringUtility_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UEdgeLoopClusteringUtility, UEdgeLoopClusteringUtility::StaticClass, TEXT("UEdgeLoopClusteringUtility"), &Z_Registration_Info_UClass_UEdgeLoopClusteringUtility, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UEdgeLoopClusteringUtility), 160231643U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UEdgeLoopClusteringUtility_h_3658755246(TEXT("/Script/SkelMeshSliceTool"),
		Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UEdgeLoopClusteringUtility_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UEdgeLoopClusteringUtility_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UEdgeLoopClusteringUtility_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UEdgeLoopClusteringUtility_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
