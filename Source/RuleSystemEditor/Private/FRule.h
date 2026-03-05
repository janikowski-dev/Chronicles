#pragma once

#include "FRule.generated.h"

USTRUCT()
struct FRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName Name;

	UPROPERTY(VisibleAnywhere)
	FGuid Id = FGuid::NewGuid();
};