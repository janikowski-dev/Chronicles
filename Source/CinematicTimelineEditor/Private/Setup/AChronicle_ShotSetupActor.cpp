#include "AChronicle_ShotSetupActor.h"
#include "AChronicle_CameraPoint.h"
#include "AChronicle_ParticipantPoint.h"
#include "UChronicle_ShotPresetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Interfaces/IPluginManager.h"
#include "UObject/Package.h"
#include "UObject/SavePackage.h"

AChronicle_ShotSetupActor::AChronicle_ShotSetupActor()
{
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    PrimaryActorTick.bCanEverTick = false;
    RootComponent = Root;

    if (!Id.IsValid())
    {
        Id = FGuid::NewGuid();
    }

    DebugMesh = CreateEditorOnlyDefaultSubobject<UStaticMeshComponent>(TEXT("DebugMesh"));
    DebugMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DebugMesh->SetupAttachment(RootComponent);
    DebugMesh->SetHiddenInGame(true);
}

void AChronicle_ShotSetupActor::PostInitProperties()
{
    Super::PostInitProperties();
    AssignDebugMesh();
}

void AChronicle_ShotSetupActor::ExportToDataAsset() const
{
    AChronicle_CameraPoint* ResponseCamera;
    TArray<AChronicle_CameraPoint*> Cameras;
    TArray<AChronicle_ParticipantPoint*> Participants;
    TArray<AActor*> Attached;
    
    GetAttachedActors(Attached);
    CollectAttachedPoints(Attached, Cameras, Participants, ResponseCamera);
    ExportToDataAsset(Cameras, Participants, ResponseCamera);
}

void AChronicle_ShotSetupActor::ExportToDataAsset(
    TArray<AChronicle_CameraPoint*>& Cameras,
    TArray<AChronicle_ParticipantPoint*>& Participants,
    AChronicle_CameraPoint*& ResponseCamera
) const
{
    if (PresetName.IsEmpty())
    {
        return;
    }
    
    const FString PackagePath = FString::Printf(TEXT("/Game/Cinematics/Presets/%s"), *PresetName);
    UPackage* Package = CreatePackage(*PackagePath);

    UChronicle_ShotPresetData* NewAsset = NewObject<UChronicle_ShotPresetData>(Package, *PresetName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

    for (const AChronicle_CameraPoint* Camera : Cameras)
    {
        for (const AChronicle_ParticipantPoint* Participant : Participants)
        {
            if (Camera->ShotIndex != Participant->ShotIndex)
            {
                continue;
            }

            FShotPair Pair;
            Pair.ShotIndex = Camera->ShotIndex;
            Pair.CameraTransform = Camera->GetActorTransform().GetRelativeTransform(GetActorTransform());
            Pair.ParticipantTransform = Participant->GetActorTransform().GetRelativeTransform(GetActorTransform());
            NewAsset->ShotPairs.Add(Pair);
            break;
        }
    }

    NewAsset->ResponseCameraTransform = ResponseCamera->GetRootComponent()->GetRelativeTransform();

    FAssetRegistryModule::AssetCreated(NewAsset);
    Package->MarkPackageDirty();

    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
    const FString PackageFilename = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
    UE_LOG(LogTemp, Log, TEXT("ShotSetupActor: Saving preset to: %s"), *PackageFilename);
    UPackage::SavePackage(Package, NewAsset, *PackageFilename, SaveArgs);
}

void AChronicle_ShotSetupActor::CollectAttachedPoints(
    TArray<AActor*> InAttached,
    TArray<AChronicle_CameraPoint*>& OutCameras,
    TArray<AChronicle_ParticipantPoint*>& OutParticipants,
    AChronicle_CameraPoint*& OutResponseCamera
) const
{
    for (AActor* Actor : InAttached)
    {
        if (AChronicle_CameraPoint* Camera = Cast<AChronicle_CameraPoint>(Actor))
        {
            if (Camera->bIsResponseCamera)
            {
                OutResponseCamera = Camera;
            }
            
            OutCameras.Add(Camera);
        }
        else if (AChronicle_ParticipantPoint* Participant = Cast<AChronicle_ParticipantPoint>(Actor))
        {
            OutParticipants.Add(Participant);
        }
    }
}

void AChronicle_ShotSetupActor::AssignDebugMesh() const
{
    const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("Chronicle"));
    const FString PackagePath = FString::Printf(TEXT("/%s/Gizmos/Cross.Cross"), *Plugin->GetName());
    
    if (UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr,PackagePath))
    {
        DebugMesh->SetStaticMesh(Mesh);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("DebugMesh: debug mesh not found"));
    }
}
