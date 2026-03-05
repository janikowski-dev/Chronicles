#pragma once

#include "FChronicle_Character.h"
#include "UChronicle_CharacterSettings.generated.h"

UCLASS(Config=CharacterSystemEditor, DefaultConfig, meta=(DisplayName="Characters"))
class CHARACTERSYSTEMEDITOR_API UChronicle_CharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	virtual FName GetCategoryName() const override;

public:
	UPROPERTY(Config, EditAnywhere, meta=(DisplayPriority=1))
	TArray<FChronicle_Character> NonPlayableCharacters;
	
	UPROPERTY(Config, EditAnywhere, meta=(DisplayPriority=0))
	TArray<FChronicle_Character> PlayableCharacters;
};