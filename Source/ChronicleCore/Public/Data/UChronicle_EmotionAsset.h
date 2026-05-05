#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UChronicle_EmotionAsset.generated.h"

UCLASS(BlueprintType, Blueprintable)
class CHRONICLECORE_API UChronicle_EmotionAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UChronicle_EmotionAsset()
	{
		if (Id.IsValid())
		{
			return;
		}

		Id = FGuid::NewGuid();
	}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
};