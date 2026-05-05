#include "AChronicle_CameraPoint.h"

#include "CineCameraComponent.h"

AChronicle_CameraPoint::AChronicle_CameraPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (UCineCameraComponent* Camera = GetCineCameraComponent())
	{
		Camera->CurrentFocalLength = 75.0f;
		Camera->CurrentAperture = 2.8f;
		Camera->Filmback.SensorWidth = 36.0f;
		Camera->Filmback.SensorHeight = 20.25f;
		Camera->FocusSettings.FocusMethod = ECameraFocusMethod::DoNotOverride;
	}

	PrimaryActorTick.bCanEverTick = false;
}
