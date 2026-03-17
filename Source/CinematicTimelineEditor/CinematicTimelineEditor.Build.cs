using UnrealBuildTool;

public class CinematicTimelineEditor : ModuleRules
{
	public CinematicTimelineEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine"
		]);
		
		PrivateDependencyModuleNames.AddRange([
			"ChronicleCore",
			"LevelSequence",
			"AssetRegistry",
			"ToolMenus",
			"Slate",
			"SlateCore",
			"DeveloperSettings",
			"UnrealEd"
		]);
	}
}