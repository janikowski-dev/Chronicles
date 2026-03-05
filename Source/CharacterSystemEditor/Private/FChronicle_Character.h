#pragma once

#include "FChronicle_Character.generated.h"

USTRUCT()
struct FChronicle_Character
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName Name;

	UPROPERTY(VisibleAnywhere)
	FGuid Id = FGuid::NewGuid();
};