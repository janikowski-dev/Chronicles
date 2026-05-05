#pragma once

#include "FChronicle_SequenceData.h"
#include "AChronicle_CharacterActor.h"
#include "Setup/UChronicle_ShotPresetData.h"
#include "UChronicle_CinematicData.generated.h"

UCLASS(BlueprintType, Blueprintable)
class CINEMATICTIMELINEEDITOR_API UChronicle_CinematicData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UChronicle_CinematicData()
	{
		if (Id.IsValid())
		{
			return;
		}
		
		Id = FGuid::NewGuid();
	}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, TSoftClassPtr<AChronicle_CharacterActor>> ActorsById;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, TSoftObjectPtr<USoundBase>> SoundsByLine;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FChronicle_SequenceData> SequencesData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FGuid> ParticipantIds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UChronicle_ShotPresetData> PresetData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FGuid> LineNodeIds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid Id;
};
