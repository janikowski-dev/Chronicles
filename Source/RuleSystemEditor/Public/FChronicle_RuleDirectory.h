#pragma once

enum class EChronicle_RuleParameterType : uint8;
class FChronicle_RuleSet;

class RULESYSTEMEDITOR_API FChronicle_RuleDirectory
{
public:
	static FChronicle_RuleSet& GetConditions(const EChronicle_RuleParameterType& ParameterType);
	static FChronicle_RuleSet& GetCallbacks(const EChronicle_RuleParameterType& ParameterType);
	static void Refresh();

private:
	static FChronicle_RuleSet RawConditionSet;
	static FChronicle_RuleSet IntConditionSet;
	static FChronicle_RuleSet CharacterConditionSet;

	static FChronicle_RuleSet RawCallbackSet;
	static FChronicle_RuleSet IntCallbackSet;
	static FChronicle_RuleSet CharacterCallbackSet;
};