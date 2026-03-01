using UnrealBuildTool;

public class RuleSystemEditor : ModuleRules
{
	public RuleSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine"
		]);

		PrivateDependencyModuleNames.AddRange([
			"ChronicleCore",
			"AssetRegistry",
			"AssetTools",
			"UnrealEd",
			"DeveloperSettings"
		]);
	}
}
