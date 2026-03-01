#include "URuleSettings.h"

#include "RuleSystem/ERuleParameterType.h"

FName URuleSettings::GetCategoryName() const
{
	return TEXT("Plugins");
}

void URuleSettings::PostInitProperties()
{
	Super::PostInitProperties();
	
	RawConditionSet = FRuleSet(RawConditions, ERuleParameterType::None);
	CharacterConditionSet = FRuleSet(CharacterConditions, ERuleParameterType::CharacterId);
	IntConditionSet = FRuleSet(IntConditions, ERuleParameterType::Integer);

	RawCallbackSet = FRuleSet(RawCallbacks, ERuleParameterType::None);
	IntCallbackSet = FRuleSet(IntCallbacks, ERuleParameterType::Integer);
	CharacterCallbackSet = FRuleSet(CharacterCallbacks, ERuleParameterType::CharacterId);
}

FRuleSet URuleSettings::GetConditions(const ERuleParameterType& ParameterType) const
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

FRuleSet URuleSettings::GetCallbacks(const ERuleParameterType& ParameterType) const
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
