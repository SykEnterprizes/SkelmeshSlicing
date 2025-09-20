// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FEdgeLoopInfoArrayWrapper;
class AActor;
class USkeletalMesh;
struct FEdgeLoopInfo;
struct FCapSkinningInfo;
#ifdef SKELMESHSLICETOOL_ULimbExtractorTool_generated_h
#error "ULimbExtractorTool.generated.h already included, missing '#pragma once' in ULimbExtractorTool.h"
#endif
#define SKELMESHSLICETOOL_ULimbExtractorTool_generated_h

#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_22_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FEdgeLoopInfoArrayWrapper_Statics; \
	SKELMESHSLICETOOL_API static class UScriptStruct* StaticStruct();


template<> SKELMESHSLICETOOL_API UScriptStruct* StaticStruct<struct FEdgeLoopInfoArrayWrapper>();

#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_28_DELEGATE \
struct _Script_SkelMeshSliceTool_eventCapMenuPrompt_Parms \
{ \
	FEdgeLoopInfoArrayWrapper EdgeLoopInfoArray; \
}; \
static inline void FCapMenuPrompt_DelegateWrapper(const FMulticastScriptDelegate& CapMenuPrompt, FEdgeLoopInfoArrayWrapper EdgeLoopInfoArray) \
{ \
	_Script_SkelMeshSliceTool_eventCapMenuPrompt_Parms Parms; \
	Parms.EdgeLoopInfoArray=EdgeLoopInfoArray; \
	CapMenuPrompt.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_SPARSE_DATA
#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execGetOwnerActor); \
	DECLARE_FUNCTION(execLoggingAndDebugSwitches); \
	DECLARE_FUNCTION(execClearExcusionBoneList); \
	DECLARE_FUNCTION(execSetExclusionBones); \
	DECLARE_FUNCTION(execSetMinimumWeightThreshold); \
	DECLARE_FUNCTION(execPopulateSkelHierachy); \
	DECLARE_FUNCTION(execLogBoneIndices); \
	DECLARE_FUNCTION(execCreateFadeRingAndCap); \
	DECLARE_FUNCTION(execMergeCapsWithMesh); \
	DECLARE_FUNCTION(execCreateReverseCapPreview); \
	DECLARE_FUNCTION(execSpawnRevCapActor); \
	DECLARE_FUNCTION(execSpawnSkelMeshActor); \
	DECLARE_FUNCTION(execSpawnProcMeshActor); \
	DECLARE_FUNCTION(execOnEditorUtilityClosed); \
	DECLARE_FUNCTION(execExtractBoneToMesh);


#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execGetOwnerActor); \
	DECLARE_FUNCTION(execLoggingAndDebugSwitches); \
	DECLARE_FUNCTION(execClearExcusionBoneList); \
	DECLARE_FUNCTION(execSetExclusionBones); \
	DECLARE_FUNCTION(execSetMinimumWeightThreshold); \
	DECLARE_FUNCTION(execPopulateSkelHierachy); \
	DECLARE_FUNCTION(execLogBoneIndices); \
	DECLARE_FUNCTION(execCreateFadeRingAndCap); \
	DECLARE_FUNCTION(execMergeCapsWithMesh); \
	DECLARE_FUNCTION(execCreateReverseCapPreview); \
	DECLARE_FUNCTION(execSpawnRevCapActor); \
	DECLARE_FUNCTION(execSpawnSkelMeshActor); \
	DECLARE_FUNCTION(execSpawnProcMeshActor); \
	DECLARE_FUNCTION(execOnEditorUtilityClosed); \
	DECLARE_FUNCTION(execExtractBoneToMesh);


#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesULimbExtractorTool(); \
	friend struct Z_Construct_UClass_ULimbExtractorTool_Statics; \
public: \
	DECLARE_CLASS(ULimbExtractorTool, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/SkelMeshSliceTool"), NO_API) \
	DECLARE_SERIALIZER(ULimbExtractorTool)


#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_INCLASS \
private: \
	static void StaticRegisterNativesULimbExtractorTool(); \
	friend struct Z_Construct_UClass_ULimbExtractorTool_Statics; \
public: \
	DECLARE_CLASS(ULimbExtractorTool, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/SkelMeshSliceTool"), NO_API) \
	DECLARE_SERIALIZER(ULimbExtractorTool)


#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ULimbExtractorTool(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ULimbExtractorTool) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ULimbExtractorTool); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ULimbExtractorTool); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ULimbExtractorTool(ULimbExtractorTool&&); \
	NO_API ULimbExtractorTool(const ULimbExtractorTool&); \
public:


#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ULimbExtractorTool(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ULimbExtractorTool(ULimbExtractorTool&&); \
	NO_API ULimbExtractorTool(const ULimbExtractorTool&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ULimbExtractorTool); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ULimbExtractorTool); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ULimbExtractorTool)


#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_103_PROLOG
#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_SPARSE_DATA \
	FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_RPC_WRAPPERS \
	FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_INCLASS \
	FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_SPARSE_DATA \
	FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_INCLASS_NO_PURE_DECLS \
	FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h_106_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> SKELMESHSLICETOOL_API UClass* StaticClass<class ULimbExtractorTool>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ULimbExtractorTool_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
