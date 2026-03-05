#pragma once

#include "FChronicle_Rule.generated.h"

USTRUCT()
struct FChronicle_Rule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName Name;

	UPROPERTY(VisibleAnywhere)
	FGuid Id = FGuid::NewGuid();
};