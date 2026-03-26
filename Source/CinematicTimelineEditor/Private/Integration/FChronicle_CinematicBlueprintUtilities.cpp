#include "FChronicle_CinematicBlueprintUtilities.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "CineCameraActor.h"
#include "MovieScene.h"
#include "Animation/SkeletalMeshActor.h"
#include "Sections/MovieSceneAudioSection.h"
#include "Sections/MovieSceneCameraCutSection.h"
#include "Tracks/MovieSceneCameraCutTrack.h"
#include "Tracks/MovieSceneAudioTrack.h"
#include "Tracks/MovieSceneFloatTrack.h"
#include "Tracks/MovieSceneSpawnTrack.h"

void FChronicle_CinematicBlueprintUtilities::InitSequence(
	ULevelSequence* LevelSequence,
	const UChronicle_CinematicData* CinematicData,
	const FChronicle_SequenceData& SequenceData
)
{
	UMovieScene* MovieScene = nullptr;
	
	if (TryGetMovieScene(LevelSequence, MovieScene))
	{
		const FSequenceInfo Info = ConvertToInfo(LevelSequence, MovieScene, CinematicData, SequenceData);
		PopulateCameraCutTrack(MovieScene, Info);
		PopulateAudioTrack(MovieScene, Info);
		ApplyInfo(MovieScene, Info);
	}

	ApplyChanges(LevelSequence);
}

TSoftObjectPtr<UWorld> FChronicle_CinematicBlueprintUtilities::ToWorldPointer(const FString Path)
{
	return TSoftObjectPtr<UWorld>(FSoftObjectPath(Path));
}

UBlueprint* FChronicle_CinematicBlueprintUtilities::CreateBlueprintFromParent(
	UClass* ParentClass,
	const FString& PackagePath,
	const FString& BlueprintName,
	const UChronicle_CinematicData* Data
)
{
	if (!ParentClass)
	{
		return nullptr;
	}

	const FString FullPath = PackagePath / BlueprintName;
	UPackage* Package = CreatePackage(*FullPath);

	if (UObject* ExistingObj = StaticFindObject(UBlueprint::StaticClass(), Package, *BlueprintName))
	{
		return Cast<UBlueprint>(ExistingObj);
	}
	
	UBlueprint* NewBlueprint = FKismetEditorUtilities::CreateBlueprint(
		ParentClass,
		Package,
		*BlueprintName,
		BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass()
	);
	
	if (!NewBlueprint)
	{
		return nullptr;
	}
	
	FKismetEditorUtilities::CompileBlueprint(NewBlueprint);
	
	if (NewBlueprint->GeneratedClass)
	{
		if (UObject* CDO = NewBlueprint->GeneratedClass->GetDefaultObject())
		{
			FProperty* Prop = FindFProperty<FProperty>(NewBlueprint->GeneratedClass, TEXT("Data"));
	
			if (FObjectProperty* ObjProp = CastField<FObjectProperty>(Prop))
			{
				ObjProp->SetObjectPropertyValue_InContainer(CDO, const_cast<UChronicle_CinematicData*>(Data));
				FPropertyChangedEvent PropertyChangedEvent(ObjProp);
				CDO->PostEditChangeProperty(PropertyChangedEvent);
				NewBlueprint->Modify();
				FBlueprintEditorUtils::MarkBlueprintAsModified(NewBlueprint);
			}
		}
	}
	
	FAssetRegistryModule::AssetCreated(NewBlueprint);
	NewBlueprint->MarkPackageDirty();
	Package->MarkPackageDirty();
	return NewBlueprint;
}

bool FChronicle_CinematicBlueprintUtilities::TryGetMovieScene(const ULevelSequence* LevelSequence, UMovieScene*& MovieScene)
{
	if (!LevelSequence)
	{
		return false;
	}

	MovieScene = LevelSequence->GetMovieScene();
	
	if (MovieScene == nullptr)
	{
		return false;
	}
	
	MovieScene->SetClockSource(EUpdateClockSource::Audio);
	return true;
}

void FChronicle_CinematicBlueprintUtilities::ApplyInfo(UMovieScene* MovieScene, const FSequenceInfo& SequenceInfo)
{
	MovieScene->SetPlaybackRange(TRange<FFrameNumber>(0, SequenceInfo.TotalFrameCount));
}

FSequenceInfo FChronicle_CinematicBlueprintUtilities::ConvertToInfo(
	ULevelSequence* LevelSequence,
	UMovieScene* MovieScene,
	const UChronicle_CinematicData* CinematicData,
	const FChronicle_SequenceData& SequenceData
)
{
	FFrameNumber FrameCounter = 0;
	FSequenceInfo SequenceInfo;

	for (const FChronicle_DialogueNodeData& Node : SequenceData.Nodes)
	{
		FTrackInfo TrackInfo;
		
		const USoundBase* Sound = CinematicData->SoundsByLine[Node.Id].LoadSynchronous();
		const FFrameRate TickResolution = MovieScene->GetTickResolution();
		const double SoundDuration = Sound->GetDuration();
		const FFrameNumber FrameDuration = (SoundDuration * TickResolution).FloorToFrame();

		TrackInfo.Sound = CinematicData->SoundsByLine[Node.Id];
		TrackInfo.StartFrame = FFrameNumber(FrameCounter);
		TrackInfo.EndFrame = FrameDuration + FrameCounter;
		TrackInfo.ParticipantId = Node.SpeakerId;

		SequenceInfo.Tracks.Add(TrackInfo);

		FrameCounter += FrameDuration;
	}

	SequenceInfo.TotalFrameCount = FrameCounter;
	
	const int ParticipantCount = CinematicData->ParticipantIds.Num();

	for (int i = 0; i < ParticipantCount; i++)
	{
		constexpr float Radius = 200.0f;
		const FGuid& ParticipantId = CinematicData->ParticipantIds[i];
    
		const float Angle = 2.0f * PI * i / ParticipantCount;
		const FVector Position(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.0f);
    
		SequenceInfo.TransformByParticipantIds.Add(ParticipantId, FTransform(Position));
	}

	for (const FGuid& ParticipantId : CinematicData->ParticipantIds)
	{
		for (const FTrackInfo& Track : SequenceInfo.Tracks)
		{
			if (Track.ParticipantId != ParticipantId)
			{
				continue;
			}

			const FTransform* ParticipantTransform = SequenceInfo.TransformByParticipantIds.Find(ParticipantId);
			const FVector ParticipantLocation = ParticipantTransform->GetLocation();
			const FVector CameraLocation = FVector::ZeroVector;
			const FRotator CameraRotation = (ParticipantLocation - CameraLocation).Rotation();
			const FTransform CameraTransform(CameraRotation, CameraLocation);

			FGuid CameraId = AddCamera(LevelSequence, MovieScene, CameraTransform);
			SequenceInfo.CameraIdByParticipantIds.Add(ParticipantId, CameraId);

			USkeletalMesh* SkeletalMesh = CinematicData->ActorsById[Track.ParticipantId].LoadSynchronous();
			FGuid ModelId = AddModel(LevelSequence, MovieScene, SkeletalMesh, *ParticipantTransform);
			SequenceInfo.ModelIdByParticipantIds.Add(ParticipantId, ModelId);
			break;
		}
	}
	
	return SequenceInfo;
}

void FChronicle_CinematicBlueprintUtilities::PopulateCameraCutTrack(
	UMovieScene* MovieScene,
	const FSequenceInfo& SequenceInfo
)
{
	UMovieSceneTrack* CutTrack = MovieScene->AddCameraCutTrack(UMovieSceneCameraCutTrack::StaticClass());
	CutTrack->SetSortingOrder(1);
	
	UMovieSceneCameraCutTrack* CameraCutTrack = Cast<UMovieSceneCameraCutTrack>(MovieScene->GetCameraCutTrack());

	for (const FTrackInfo& Track : SequenceInfo.Tracks)
	{
		UMovieSceneCameraCutSection* CameraSection = Cast<UMovieSceneCameraCutSection>(CameraCutTrack->CreateNewSection());
		CameraSection->SetRange(TRange<FFrameNumber>(Track.StartFrame, Track.EndFrame));

		FMovieSceneObjectBindingID CameraBindingID(SequenceInfo.CameraIdByParticipantIds[Track.ParticipantId]);
		CameraSection->SetCameraBindingID(CameraBindingID);
		CameraCutTrack->AddSection(*CameraSection);
	}
}

void FChronicle_CinematicBlueprintUtilities::PopulateAudioTrack(
	UMovieScene* MovieScene,
	const FSequenceInfo& SequenceInfo
)
{
	UMovieSceneTrack* AudioTrack = MovieScene->AddTrack<UMovieSceneAudioTrack>();

	if (UMovieSceneNameableTrack* Nameable = Cast<UMovieSceneNameableTrack>(AudioTrack))
	{
		Nameable->SetDisplayName(FText::FromString("Main Track"));
	}
	
	AudioTrack->SetSortingOrder(0);

	for (const FTrackInfo& Track : SequenceInfo.Tracks)
	{
		UMovieSceneAudioSection* AudioSection = Cast<UMovieSceneAudioSection>(AudioTrack->CreateNewSection());
		AudioSection->SetRange(TRange<FFrameNumber>(Track.StartFrame, Track.EndFrame));
		
		USoundBase* Sound = Track.Sound.LoadSynchronous();
		AudioTrack->AddSection(*AudioSection);
		AudioSection->SetIsLocked(true);
		AudioSection->SetSound(Sound);
	}
}

FGuid FChronicle_CinematicBlueprintUtilities::AddCamera(
	ULevelSequence* LevelSequence,
	UMovieScene* MovieScene,
	const FTransform& SpawnTransform
)
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
    
	if (!World)
	{
		return {};
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.ObjectFlags &= ~RF_Transactional;
	ACineCameraActor* CameraActor = World->SpawnActor<ACineCameraActor>(SpawnParams);
	CameraActor->SetActorTransform(SpawnTransform);

	FActorLabelUtilities::SetActorLabelUnique(CameraActor, TEXT("Camera"));

	const FGuid CameraGuid = MovieSceneHelpers::TryCreateCustomSpawnableBinding(LevelSequence, CameraActor);

	if (UMovieSceneSpawnTrack* SpawnTrack = MovieScene->AddTrack<UMovieSceneSpawnTrack>(CameraGuid))
	{
		SpawnTrack->Modify();
		SpawnTrack->AddSection(*SpawnTrack->CreateNewSection());
	}

	World->DestroyActor(CameraActor);
	return CameraGuid;
}

FGuid FChronicle_CinematicBlueprintUtilities::AddModel(
	ULevelSequence* LevelSequence,
	UMovieScene* MovieScene,
    USkeletalMesh* SkeletalMesh,
	const FTransform& SpawnTransform
)
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
    
	if (!World)
	{
		return {};
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.ObjectFlags &= ~RF_Transactional;
	ASkeletalMeshActor* MeshActor = World->SpawnActor<ASkeletalMeshActor>(SpawnParams);
	MeshActor->SetActorTransform(SpawnTransform);
	MeshActor->GetSkeletalMeshComponent()->SetSkeletalMesh(SkeletalMesh);

	FActorLabelUtilities::SetActorLabelUnique(MeshActor, SkeletalMesh->GetName());

	const FGuid MeshGuid = MovieSceneHelpers::TryCreateCustomSpawnableBinding(LevelSequence, MeshActor);

	if (UMovieSceneSpawnTrack* SpawnTrack = MovieScene->AddTrack<UMovieSceneSpawnTrack>(MeshGuid))
	{
		SpawnTrack->Modify();
		SpawnTrack->AddSection(*SpawnTrack->CreateNewSection());
	}

	World->DestroyActor(MeshActor);
	return MeshGuid;
}

void FChronicle_CinematicBlueprintUtilities::ApplyChanges(const ULevelSequence* LevelSequence)
{
	if (!LevelSequence)
	{
		return;
	}
	
	LevelSequence->MarkPackageDirty();
}
