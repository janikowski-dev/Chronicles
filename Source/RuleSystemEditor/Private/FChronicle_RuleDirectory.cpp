#include "FChronicle_RuleDirectory.h"

#include "FChronicle_RuleSet.h"
#include "EChronicle_RuleParameterType.h"
#include "UChronicle_RuleSettings.h"

FChronicle_RuleSet FChronicle_RuleDirectory::RawConditionSet;
FChronicle_RuleSet FChronicle_RuleDirectory::IntConditionSet;
FChronicle_RuleSet FChronicle_RuleDirectory::CharacterConditionSet;

FChronicle_RuleSet FChronicle_RuleDirectory::RawCallbackSet;
FChronicle_RuleSet FChronicle_RuleDirectory::IntCallbackSet;
FChronicle_RuleSet FChronicle_RuleDirectory::CharacterCallbackSet;

FChronicle_RuleSet& FChronicle_RuleDirectory::GetConditions(const EChronicle_RuleParameterType& ParameterType)
{
	switch (ParameterType)
	{
	case EChronicle_RuleParameterType::None:
		return RawConditionSet;
	case EChronicle_RuleParameterType::CharacterId:
		return CharacterConditionSet;
	case EChronicle_RuleParameterType::Integer:
		return IntConditionSet;
	}
	
	return RawConditionSet;
}

FChronicle_RuleSet& FChronicle_RuleDirectory::GetCallbacks(const EChronicle_RuleParameterType& ParameterType)
{
	switch (ParameterType)
	{
	case EChronicle_RuleParameterType::None:
		return RawCallbackSet;
	case EChronicle_RuleParameterType::CharacterId:
		return CharacterCallbackSet;
	case EChronicle_RuleParameterType::Integer:
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
