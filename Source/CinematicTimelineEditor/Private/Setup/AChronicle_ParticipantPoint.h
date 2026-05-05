#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AChronicle_ParticipantPoint.generated.h"

UCLASS()
class CINEMATICTIMELINEEDITOR_API AChronicle_ParticipantPoint : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UStaticMeshComponent* DebugMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup", meta=(ToolTip="Shot with index 0 is associated with player if present on the set."))
	int32 ShotIndex;

public:
	AChronicle_ParticipantPoint();

	virtual void PostInitProperties() override;

private:
	void AssignDebugMesh() const;
};