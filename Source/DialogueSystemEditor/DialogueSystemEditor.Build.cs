using UnrealBuildTool;

public class DialogueSystemEditor : ModuleRules
{
	public DialogueSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine"
		]);
		
		PrivateDependencyModuleNames.AddRange([
			"ChronicleCore",
			"RuleSystemEditor",
			"CharacterSystemEditor",
			"Core",
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"AssetTools",
			"EditorFramework",
			"Slate",
			"SlateCore",
			"GraphEditor",
			"ToolMenus",
			"InputCore",
			"WorkspaceMenuStructure",
			"GameplayTags",
			"GameplayTagsEditor",
			"EditorStyle",
			"Projects",
			"JsonUtilities",
			"ApplicationCore"
		]);
	}
}
