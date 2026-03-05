#pragma once

#include "FChronicleCharacter.generated.h"

USTRUCT()
struct FChronicleCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName Name;

	UPROPERTY(VisibleAnywhere)
	FGuid Id = FGuid::NewGuid();
};