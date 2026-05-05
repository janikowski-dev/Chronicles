#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AChronicle_ShotSetupActor.generated.h"

class AChronicle_CameraPoint;
class AChronicle_ParticipantPoint;

UCLASS()
class CINEMATICTIMELINEEDITOR_API AChronicle_ShotSetupActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UStaticMeshComponent* DebugMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Setup")
	FGuid Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	FString PresetName;

public:
	AChronicle_ShotSetupActor();

	virtual void PostInitProperties() override;

	UFUNCTION(CallInEditor, BlueprintCallable, Category="Setup")
	void ExportToDataAsset() const;

private:
	void ExportToDataAsset(
		TArray<AChronicle_CameraPoint*>& Cameras,
		TArray<AChronicle_ParticipantPoint*>& Participants,
		AChronicle_CameraPoint*& ResponseCamera
	) const;
	
	void CollectAttachedPoints(
		TArray<AActor*> InAttached,
		TArray<AChronicle_CameraPoint*>& OutCameras,
		TArray<AChronicle_ParticipantPoint*>& OutParticipants,
		AChronicle_CameraPoint*& OutResponseCamera
	) const;

	void AssignDebugMesh() const;
};