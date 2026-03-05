#pragma once

#include "FRule.h"
#include "UChronicle_RuleSettings.generated.h"

UCLASS(Config=RuleSystemEditor, DefaultConfig, meta=(DisplayName="Rules"))
class RULESYSTEMEDITOR_API UChronicle_RuleSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	virtual FName GetCategoryName() const override;

public:
	UPROPERTY(Config, EditAnywhere, Category="Conditions")
	TArray<FRule> RawConditions;
	UPROPERTY(Config, EditAnywhere, Category="Conditions")
	TArray<FRule> IntConditions;
	UPROPERTY(Config, EditAnywhere, Category="Conditions")
	TArray<FRule> CharacterConditions;

	UPROPERTY(Config, EditAnywhere, Category="Callbacks")
	TArray<FRule> RawCallbacks;
	UPROPERTY(Config, EditAnywhere, Category="Callbacks")
	TArray<FRule> IntCallbacks;
	UPROPERTY(Config, EditAnywhere, Category="Callbacks")
	TArray<FRule> CharacterCallbacks;
};