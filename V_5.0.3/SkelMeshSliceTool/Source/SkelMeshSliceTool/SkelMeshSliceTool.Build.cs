// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SkelMeshSliceTool : ModuleRules
{
	public SkelMeshSliceTool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "ProceduralMeshComponent", "MeshDescription", "StaticMeshDescription", "MeshConversion", "DesktopPlatform",  "SkeletalMeshEditor"

				// ... add other public dependencies that you statically link with here ...
			}
            );
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",      
            	"SkeletalMeshUtilitiesCommon"

				// ... add private dependencies that you statically link with here ...	
			}
            );
        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.Add("AssetTools");
        }


        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
