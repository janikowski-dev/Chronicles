#pragma once

#include "FChronicle_SequenceData.h"
#include "Animation/SkeletalMeshActor.h"
#include "UChronicle_CinematicData.generated.h"

UCLASS(BlueprintType, Blueprintable)
class CINEMATICTIMELINEEDITOR_API UChronicle_CinematicData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, TSoftClassPtr<ASkeletalMeshActor>> ActorsById;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, TSoftObjectPtr<USoundBase>> SoundsByLine;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FChronicle_SequenceData> SequencesData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FGuid> ParticipantIds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FGuid> LineNodeIds;
};
