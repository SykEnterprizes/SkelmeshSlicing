// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef SKELMESHSLICETOOL_ESkinningType_generated_h
#error "ESkinningType.generated.h already included, missing '#pragma once' in ESkinningType.h"
#endif
#define SKELMESHSLICETOOL_ESkinningType_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_HostProject_Plugins_SkelMeshSliceTool_Source_SkelMeshSliceTool_Public_ESkinningType_h


#define FOREACH_ENUM_ESKINNINGTYPE(op) \
	op(ESkinningType::ProximityBlending) \
	op(ESkinningType::RigidClosestBone) 

enum class ESkinningType : uint8;
template<> SKELMESHSLICETOOL_API UEnum* StaticEnum<ESkinningType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
