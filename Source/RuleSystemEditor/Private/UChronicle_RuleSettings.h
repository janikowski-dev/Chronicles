#pragma once

#include "FChronicle_Rule.h"
#include "UChronicle_RuleSettings.generated.h"

UCLASS(Config=RuleSystemEditor, DefaultConfig, meta=(DisplayName="Rules"))
class RULESYSTEMEDITOR_API UChronicle_RuleSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	virtual FName GetCategoryName() const override;

public:
	UPROPERTY(Config, EditAnywhere, Category="Conditions")
	TArray<FChronicle_Rule> RawConditions;
	UPROPERTY(Config, EditAnywhere, Category="Conditions")
	TArray<FChronicle_Rule> IntConditions;
	UPROPERTY(Config, EditAnywhere, Category="Conditions")
	TArray<FChronicle_Rule> CharacterConditions;

	UPROPERTY(Config, EditAnywhere, Category="Callbacks")
	TArray<FChronicle_Rule> RawCallbacks;
	UPROPERTY(Config, EditAnywhere, Category="Callbacks")
	TArray<FChronicle_Rule> IntCallbacks;
	UPROPERTY(Config, EditAnywhere, Category="Callbacks")
	TArray<FChronicle_Rule> CharacterCallbacks;
};