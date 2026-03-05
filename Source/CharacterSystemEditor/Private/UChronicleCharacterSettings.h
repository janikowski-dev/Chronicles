#pragma once

#include "FChronicleCharacter.h"
#include "UChronicleCharacterSettings.generated.h"

UCLASS(Config=CharacterSystemEditor, DefaultConfig, meta=(DisplayName="Characters"))
class CHARACTERSYSTEMEDITOR_API UChronicleCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	virtual FName GetCategoryName() const override;

public:
	UPROPERTY(Config, EditAnywhere, meta=(DisplayPriority=1))
	TArray<FChronicleCharacter> NonPlayableCharacters;
	
	UPROPERTY(Config, EditAnywhere, meta=(DisplayPriority=0))
	TArray<FChronicleCharacter> PlayableCharacters;
};