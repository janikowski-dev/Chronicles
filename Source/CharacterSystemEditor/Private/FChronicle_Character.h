#pragma once

#include "FChronicle_CharacterModel.h"
#include "FChronicle_Character.generated.h"

USTRUCT()
struct FChronicle_Character
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FGuid Id = FGuid::NewGuid();

	UPROPERTY(EditAnywhere)
	FName Name;

	UPROPERTY(EditAnywhere)
	TArray<FChronicle_CharacterModel> Models;
};