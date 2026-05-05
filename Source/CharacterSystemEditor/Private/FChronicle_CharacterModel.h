#pragma once

#include "AChronicle_CharacterActor.h"
#include "FChronicle_CharacterModel.generated.h"

USTRUCT(BlueprintType)
struct FChronicle_CharacterModel
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid Id = FGuid::NewGuid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AChronicle_CharacterActor> Model;
};