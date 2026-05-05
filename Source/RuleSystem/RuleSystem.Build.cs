using UnrealBuildTool;

public class RuleSystem : ModuleRules
{
	public RuleSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine"
		]);

		PrivateDependencyModuleNames.AddRange([
			"ChronicleCore"
		]);
	}
}
