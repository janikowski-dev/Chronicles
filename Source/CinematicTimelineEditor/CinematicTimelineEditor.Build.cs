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
			"CinematicTimeline",
			"CharacterSystemEditor",
			"LevelSequence",
			"AssetRegistry",
			"ToolMenus",
			"Slate",
			"SlateCore",
			"DeveloperSettings",
			"UnrealEd",
			"CinematicCamera",
			"LevelSequenceEditor",
			"MovieSceneTracks",
			"MovieSceneTools",
			"Sequencer",
			"MovieScene"
		]);
	}
}