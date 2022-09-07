// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BirdBrawler : ModuleRules
{
	public BirdBrawler( ReadOnlyTargetRules Target ) : base( Target )
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange( new string[]
			{"Core", "CoreUObject", "Engine", "InputCore", "StackBasedFSM", "UMG", "SlateCore"} );
	}
}