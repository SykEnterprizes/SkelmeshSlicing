// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SkelMeshSliceTool/Public/BoneHierachyStruct.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBoneHierachyStruct() {}
// Cross Module References
	SKELMESHSLICETOOL_API UScriptStruct* Z_Construct_UScriptStruct_FBoneHierachyStruct();
	UPackage* Z_Construct_UPackage__Script_SkelMeshSliceTool();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_BoneHierachyStruct;
class UScriptStruct* FBoneHierachyStruct::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_BoneHierachyStruct.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_BoneHierachyStruct.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FBoneHierachyStruct, Z_Construct_UPackage__Script_SkelMeshSliceTool(), TEXT("BoneHierachyStruct"));
	}
	return Z_Registration_Info_UScriptStruct_BoneHierachyStruct.OuterSingleton;
}
template<> SKELMESHSLICETOOL_API UScriptStruct* StaticStruct<FBoneHierachyStruct>()
{
	return FBoneHierachyStruct::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Bone_MetaData[];
#endif
		static const UECodeGen_Private::FNamePropertyParams NewProp_Bone;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ParentBone_MetaData[];
#endif
		static const UECodeGen_Private::FNamePropertyParams NewProp_ParentBone;
		static const UECodeGen_Private::FNamePropertyParams NewProp_ChildBones_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ChildBones_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_ChildBones;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BoneIndex_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_BoneIndex;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BoneTransform_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_BoneTransform;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BoneVisibility_MetaData[];
#endif
		static void NewProp_BoneVisibility_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_BoneVisibility;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_IsExpanded_MetaData[];
#endif
		static void NewProp_IsExpanded_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_IsExpanded;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/BoneHierachyStruct.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBoneHierachyStruct>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_Bone_MetaData[] = {
		{ "Category", "Limb Tools|BoneHierachy" },
		{ "ModuleRelativePath", "Public/BoneHierachyStruct.h" },
	};
#endif
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_Bone = { "Bone", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FBoneHierachyStruct, Bone), METADATA_PARAMS(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_Bone_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_Bone_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ParentBone_MetaData[] = {
		{ "Category", "Limb Tools|BoneHierachy" },
		{ "ModuleRelativePath", "Public/BoneHierachyStruct.h" },
	};
#endif
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ParentBone = { "ParentBone", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FBoneHierachyStruct, ParentBone), METADATA_PARAMS(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ParentBone_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ParentBone_MetaData)) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ChildBones_Inner = { "ChildBones", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ChildBones_MetaData[] = {
		{ "Category", "Limb Tools|BoneHierachy" },
		{ "ModuleRelativePath", "Public/BoneHierachyStruct.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ChildBones = { "ChildBones", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FBoneHierachyStruct, ChildBones), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ChildBones_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ChildBones_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneIndex_MetaData[] = {
		{ "Category", "Limb Tools|BoneHierachy" },
		{ "ModuleRelativePath", "Public/BoneHierachyStruct.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneIndex = { "BoneIndex", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FBoneHierachyStruct, BoneIndex), METADATA_PARAMS(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneIndex_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneIndex_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneTransform_MetaData[] = {
		{ "Category", "Limb Tools|BoneHierachy" },
		{ "ModuleRelativePath", "Public/BoneHierachyStruct.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneTransform = { "BoneTransform", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FBoneHierachyStruct, BoneTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneTransform_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneTransform_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneVisibility_MetaData[] = {
		{ "Category", "Limb Tools|BoneHierachy" },
		{ "ModuleRelativePath", "Public/BoneHierachyStruct.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneVisibility_SetBit(void* Obj)
	{
		((FBoneHierachyStruct*)Obj)->BoneVisibility = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneVisibility = { "BoneVisibility", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FBoneHierachyStruct), &Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneVisibility_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneVisibility_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneVisibility_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_IsExpanded_MetaData[] = {
		{ "Category", "Limb Tools|BoneHierachy" },
		{ "ModuleRelativePath", "Public/BoneHierachyStruct.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_IsExpanded_SetBit(void* Obj)
	{
		((FBoneHierachyStruct*)Obj)->IsExpanded = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_IsExpanded = { "IsExpanded", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(FBoneHierachyStruct), &Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_IsExpanded_SetBit, METADATA_PARAMS(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_IsExpanded_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_IsExpanded_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_Bone,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ParentBone,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ChildBones_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_ChildBones,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneIndex,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneTransform,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_BoneVisibility,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewProp_IsExpanded,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_SkelMeshSliceTool,
		nullptr,
		&NewStructOps,
		"BoneHierachyStruct",
		sizeof(FBoneHierachyStruct),
		alignof(FBoneHierachyStruct),
		Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FBoneHierachyStruct()
	{
		if (!Z_Registration_Info_UScriptStruct_BoneHierachyStruct.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_BoneHierachyStruct.InnerSingleton, Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_BoneHierachyStruct.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_BoneHierachyStruct_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_BoneHierachyStruct_h_Statics::ScriptStructInfo[] = {
		{ FBoneHierachyStruct::StaticStruct, Z_Construct_UScriptStruct_FBoneHierachyStruct_Statics::NewStructOps, TEXT("BoneHierachyStruct"), &Z_Registration_Info_UScriptStruct_BoneHierachyStruct, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FBoneHierachyStruct), 3251922161U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_BoneHierachyStruct_h_1601742179(TEXT("/Script/SkelMeshSliceTool"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_BoneHierachyStruct_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_BoneHierachyStruct_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
