// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SkelMeshSliceTool/Public/ESkinningType.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeESkinningType() {}
// Cross Module References
	SKELMESHSLICETOOL_API UEnum* Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType();
	UPackage* Z_Construct_UPackage__Script_SkelMeshSliceTool();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ESkinningType;
	static UEnum* ESkinningType_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ESkinningType.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ESkinningType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType, Z_Construct_UPackage__Script_SkelMeshSliceTool(), TEXT("ESkinningType"));
		}
		return Z_Registration_Info_UEnum_ESkinningType.OuterSingleton;
	}
	template<> SKELMESHSLICETOOL_API UEnum* StaticEnum<ESkinningType>()
	{
		return ESkinningType_StaticEnum();
	}
	struct Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType_Statics::Enumerators[] = {
		{ "ESkinningType::ProximityBlending", (int64)ESkinningType::ProximityBlending },
		{ "ESkinningType::RigidClosestBone", (int64)ESkinningType::RigidClosestBone },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType_Statics::Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/ESkinningType.h" },
		{ "ProximityBlending.Category", "Limb Tools|CapSkinningType" },
		{ "ProximityBlending.DisplayName", "Proximity Blending" },
		{ "ProximityBlending.Name", "ESkinningType::ProximityBlending" },
		{ "RigidClosestBone.Category", "Limb Tools|CapSkinningType" },
		{ "RigidClosestBone.DisplayName", "Rigid Closest Bone" },
		{ "RigidClosestBone.Name", "ESkinningType::RigidClosestBone" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_SkelMeshSliceTool,
		nullptr,
		"ESkinningType",
		"ESkinningType",
		Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType()
	{
		if (!Z_Registration_Info_UEnum_ESkinningType.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ESkinningType.InnerSingleton, Z_Construct_UEnum_SkelMeshSliceTool_ESkinningType_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ESkinningType.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ESkinningType_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ESkinningType_h_Statics::EnumInfo[] = {
		{ ESkinningType_StaticEnum, TEXT("ESkinningType"), &Z_Registration_Info_UEnum_ESkinningType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 4077510313U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ESkinningType_h_353136797(TEXT("/Script/SkelMeshSliceTool"),
		nullptr, 0,
		nullptr, 0,
		Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ESkinningType_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ESkinningType_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
