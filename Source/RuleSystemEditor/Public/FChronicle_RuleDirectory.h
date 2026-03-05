#pragma once

enum class ERuleParameterType : uint8;
class FRuleSet;

class RULESYSTEMEDITOR_API FChronicle_RuleDirectory
{
public:
	static FRuleSet& GetConditions(const ERuleParameterType& ParameterType);
	static FRuleSet& GetCallbacks(const ERuleParameterType& ParameterType);
	static void Refresh();

private:
	static FRuleSet RawConditionSet;
	static FRuleSet IntConditionSet;
	static FRuleSet CharacterConditionSet;

	static FRuleSet RawCallbackSet;
	static FRuleSet IntCallbackSet;
	static FRuleSet CharacterCallbackSet;
};