using UnrealBuildTool;

public class CharacterSystemEditor : ModuleRules
{
	public CharacterSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine"
		]);

		PrivateDependencyModuleNames.AddRange([
			"ChronicleCore",
			"CharacterSystem",
			"AssetRegistry",
			"AssetTools",
			"UnrealEd",
			"DeveloperSettings"
		]);
	}
}