using UnrealBuildTool;

public class CharacterSystem : ModuleRules
{
	public CharacterSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine"
		]);

		PrivateDependencyModuleNames.AddRange([
		]);
	}
}
