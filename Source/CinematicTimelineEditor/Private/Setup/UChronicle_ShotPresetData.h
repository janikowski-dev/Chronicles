#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UChronicle_ShotPresetData.generated.h"

USTRUCT(BlueprintType)
struct FShotPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ShotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform CameraTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ParticipantTransform;
};

UCLASS(BlueprintType)
class CINEMATICTIMELINEEDITOR_API UChronicle_ShotPresetData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
	FTransform ResponseCameraTransform;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Preset")
	TArray<FShotPair> ShotPairs;
};