// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Dusty : ModuleRules
{
	public Dusty(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			// Default Modules
			"Core", 
			"CoreUObject", 
			"Engine",
			
			// New Modules
			"InputCore", 
			"HeadMountedDisplay", 
			"EnhancedInput",
			"GameplayTasks",
			"NavigationSystem",
			"AIModule",
			"UMG",
			"GeometryCollectionEngine",
            "FieldSystemEngine",
			"Niagara",
			"LevelSequence",
			"MovieScene",
			"MovieSceneTracks",
			"GameplayCameras",
			"AssetRegistry",
        });

		PrivateDependencyModuleNames.AddRange(new string[] { "Navmesh"/*, "UnrealEd"*/ });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		if (Target.bBuildEditor) // only work when in editor
		{
			PublicDependencyModuleNames.Add("UnrealEd");
		}
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
