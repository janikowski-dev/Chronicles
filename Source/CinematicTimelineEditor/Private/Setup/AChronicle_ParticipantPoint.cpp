#include "AChronicle_ParticipantPoint.h"

#include "Interfaces/IPluginManager.h"

AChronicle_ParticipantPoint::AChronicle_ParticipantPoint()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = Root;

	DebugMesh = CreateEditorOnlyDefaultSubobject<UStaticMeshComponent>(TEXT("DebugMesh"));
    DebugMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DebugMesh->SetupAttachment(RootComponent);
	DebugMesh->SetHiddenInGame(true);
}

void AChronicle_ParticipantPoint::PostInitProperties()
{
	Super::PostInitProperties();
	AssignDebugMesh();
}

void AChronicle_ParticipantPoint::AssignDebugMesh() const
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("Chronicle"));
	const FString PackagePath = FString::Printf(TEXT("/%s/Gizmos/Spot.Spot"), *Plugin->GetName());
    
	if (UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr,PackagePath))
	{
		DebugMesh->SetStaticMesh(Mesh);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DebugMesh: debug mesh not found"));
	}
}
