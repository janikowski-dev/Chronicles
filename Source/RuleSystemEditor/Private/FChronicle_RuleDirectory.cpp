#include "FChronicle_RuleDirectory.h"

#include "FRuleSet.h"
#include "ERuleParameterType.h"
#include "UChronicle_RuleSettings.h"

FRuleSet FChronicle_RuleDirectory::RawConditionSet;
FRuleSet FChronicle_RuleDirectory::IntConditionSet;
FRuleSet FChronicle_RuleDirectory::CharacterConditionSet;

FRuleSet FChronicle_RuleDirectory::RawCallbackSet;
FRuleSet FChronicle_RuleDirectory::IntCallbackSet;
FRuleSet FChronicle_RuleDirectory::CharacterCallbackSet;

FRuleSet& FChronicle_RuleDirectory::GetConditions(const ERuleParameterType& ParameterType)
{
	switch (ParameterType)
	{
	case ERuleParameterType::None:
		return RawConditionSet;
	case ERuleParameterType::CharacterId:
		return CharacterConditionSet;
	case ERuleParameterType::Integer:
		return IntConditionSet;
	}
	
	return RawConditionSet;
}

FRuleSet& FChronicle_RuleDirectory::GetCallbacks(const ERuleParameterType& ParameterType)
{
	switch (ParameterType)
	{
	case ERuleParameterType::None:
		return RawCallbackSet;
	case ERuleParameterType::CharacterId:
		return CharacterCallbackSet;
	case ERuleParameterType::Integer:
		return IntCallbackSet;
	}
	
	return RawCallbackSet;
}

void FChronicle_RuleDirectory::Refresh()
{
	const UChronicle_RuleSettings* const Settings = GetDefault<UChronicle_RuleSettings>();
	CharacterConditionSet.Refresh(Settings->CharacterConditions);
	CharacterCallbackSet.Refresh(Settings->CharacterCallbacks);
	IntConditionSet.Refresh(Settings->IntConditions);
	IntCallbackSet.Refresh(Settings->IntCallbacks);
	RawConditionSet.Refresh(Settings->RawConditions);
	RawCallbackSet.Refresh(Settings->RawCallbacks);
}
