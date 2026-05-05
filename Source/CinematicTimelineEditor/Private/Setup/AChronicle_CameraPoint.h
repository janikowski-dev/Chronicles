#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "AChronicle_CameraPoint.generated.h"

UCLASS()
class CINEMATICTIMELINEEDITOR_API AChronicle_CameraPoint : public ACineCameraActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UStaticMeshComponent* DebugMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup", meta=(ToolTip="Shot with index 0 is associated with player if present on the set."))
	int32 ShotIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	bool bIsResponseCamera;

public:
	AChronicle_CameraPoint(const FObjectInitializer& ObjectInitializer);
};