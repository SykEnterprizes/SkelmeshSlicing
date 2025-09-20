// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SkelMeshSliceTool/Public/UModalTaskHelper.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUModalTaskHelper() {}
// Cross Module References
	SKELMESHSLICETOOL_API UClass* Z_Construct_UClass_UModalTaskHelper_NoRegister();
	SKELMESHSLICETOOL_API UClass* Z_Construct_UClass_UModalTaskHelper();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_SkelMeshSliceTool();
// End Cross Module References
	void UModalTaskHelper::StaticRegisterNativesUModalTaskHelper()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UModalTaskHelper);
	UClass* Z_Construct_UClass_UModalTaskHelper_NoRegister()
	{
		return UModalTaskHelper::StaticClass();
	}
	struct Z_Construct_UClass_UModalTaskHelper_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UModalTaskHelper_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_SkelMeshSliceTool,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UModalTaskHelper_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "UModalTaskHelper.h" },
		{ "ModuleRelativePath", "Public/UModalTaskHelper.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UModalTaskHelper_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UModalTaskHelper>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UModalTaskHelper_Statics::ClassParams = {
		&UModalTaskHelper::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UModalTaskHelper_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UModalTaskHelper_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UModalTaskHelper()
	{
		if (!Z_Registration_Info_UClass_UModalTaskHelper.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UModalTaskHelper.OuterSingleton, Z_Construct_UClass_UModalTaskHelper_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UModalTaskHelper.OuterSingleton;
	}
	template<> SKELMESHSLICETOOL_API UClass* StaticClass<UModalTaskHelper>()
	{
		return UModalTaskHelper::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UModalTaskHelper);
	struct Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UModalTaskHelper_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UModalTaskHelper_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UModalTaskHelper, UModalTaskHelper::StaticClass, TEXT("UModalTaskHelper"), &Z_Registration_Info_UClass_UModalTaskHelper, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UModalTaskHelper), 1658963178U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UModalTaskHelper_h_3736048014(TEXT("/Script/SkelMeshSliceTool"),
		Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UModalTaskHelper_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_UModalTaskHelper_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
