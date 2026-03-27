#pragma once

#include "LevelSequenceActor.h"
#include "UChronicle_CoreFunctionLibrary.generated.h"

UCLASS()
class CINEMATICTIMELINE_API UChronicle_CoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Chronicle|Cinematics")
	static void OffsetSpawnableTransforms(
		ALevelSequenceActor* LevelSequenceActor,
		const FVector& LocationOffset,
		const FRotator& RotationOffset
	);
};
