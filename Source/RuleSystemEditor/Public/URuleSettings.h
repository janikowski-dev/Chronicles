#pragma once

#include "FRule.h"
#include "FRuleSet.h"
#include "URuleSettings.generated.h"

UCLASS(Config=RuleSystemEditor, DefaultConfig, meta=(DisplayName="Rules"))
class RULESYSTEMEDITOR_API URuleSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	virtual FName GetCategoryName() const override;
	virtual void PostInitProperties() override;

	FRuleSet GetConditions(const ERuleParameterType& ParameterType) const;
	FRuleSet GetCallbacks(const ERuleParameterType& ParameterType) const;

public:
	UPROPERTY(Config, EditAnywhere, Category="Conditions")
	TArray<FRule> RawConditions;
	UPROPERTY(Config, VisibleAnywhere, Category="Conditions")
	TArray<FRule> IntConditions;
	UPROPERTY(Config, VisibleAnywhere, Category="Conditions")
	TArray<FRule> CharacterConditions;

	UPROPERTY(Config, VisibleAnywhere, Category="Callbacks")
	TArray<FRule> RawCallbacks;
	UPROPERTY(Config, VisibleAnywhere, Category="Callbacks")
	TArray<FRule> IntCallbacks;
	UPROPERTY(Config, VisibleAnywhere, Category="Callbacks")
	TArray<FRule> CharacterCallbacks;

private:
	FRuleSet RawConditionSet;
	FRuleSet IntConditionSet;
	FRuleSet CharacterConditionSet;

	FRuleSet RawCallbackSet;
	FRuleSet IntCallbackSet;
	FRuleSet CharacterCallbackSet;
};