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
#include "UObject/SavePackage.h"

FChronicle_SequenceInfo FChronicle_CinematicBlueprintUtilities::InitSequence(
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
		
		return ConvertToRuntimeInfo(LevelSequence, Info, CinematicData, SequenceData);
	}

	ApplyChanges(LevelSequence);
	return {};
}

TSoftObjectPtr<UWorld> FChronicle_CinematicBlueprintUtilities::ToWorldPointer(const FString Path)
{
	return TSoftObjectPtr<UWorld>(FSoftObjectPath(Path));
}

UBlueprint* FChronicle_CinematicBlueprintUtilities::CreateBlueprintFromParent(
	UClass* ParentClass,
	const FString& PackagePath,
	const FString& BlueprintName,
	const FChronicle_DialogueInfo& Info
)
{
	if (!Info.Id.IsValid() || !ParentClass)
	{
		return nullptr;
	}

	const FString FullPath = PackagePath / BlueprintName;
	UPackage* Package = CreatePackage(*FullPath);
	UBlueprint* Blueprint;

	if (UObject* ExistingObj = StaticFindObject(UBlueprint::StaticClass(), Package, *BlueprintName))
	{
		Blueprint = Cast<UBlueprint>(ExistingObj);
	}
	else
	{
		Blueprint = FKismetEditorUtilities::CreateBlueprint(
			ParentClass,
			Package,
			*BlueprintName,
			BPTYPE_Normal,
			UBlueprint::StaticClass(),
			UBlueprintGeneratedClass::StaticClass()
		);

		FKismetEditorUtilities::CompileBlueprint(Blueprint);
	}

	if (Blueprint->GeneratedClass)
	{
		if (UObject* CDO = Blueprint->GeneratedClass->GetDefaultObject())
		{
			FProperty* Prop = FindFProperty<FProperty>(Blueprint->GeneratedClass, TEXT("Info"));

			if (FStructProperty* StructProp = CastField<FStructProperty>(Prop))
			{
				void* StructPtr = StructProp->ContainerPtrToValuePtr<void>(CDO);
				StructProp->Struct->CopyScriptStruct(StructPtr, &Info);
				FPropertyChangedEvent PropertyChangedEvent(StructProp);
				CDO->PostEditChangeProperty(PropertyChangedEvent);
				Blueprint->Modify();
				FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
			}
		}
	}

	FAssetRegistryModule::AssetCreated(Blueprint);
	Blueprint->MarkPackageDirty();
	Package->MarkPackageDirty();

	FKismetEditorUtilities::CompileBlueprint(Blueprint);

	FString PackageFileName = FPackageName::LongPackageNameToFilename(
		Package->GetName(),
		FPackageName::GetAssetPackageExtension()
	);

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.Error = GError;

	UPackage::SavePackage(Package, Blueprint, *PackageFileName, SaveArgs);
	
	return Blueprint;
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

FChronicle_SequenceInfo FChronicle_CinematicBlueprintUtilities::ConvertToRuntimeInfo(
	const ULevelSequence* LevelSequence,
	const FSequenceInfo& SequenceInfo,
	const UChronicle_CinematicData* CinematicData,
	const FChronicle_SequenceData& SequenceData
)
{
	FChronicle_SequenceInfo RuntimeInfo;

	for (FTrackInfo Track : SequenceInfo.Tracks)
	{
		RuntimeInfo.StartFrameByNodeIds.Add(Track.Id, Track.StartFrame);
	}

	RuntimeInfo.Sequence = TSoftObjectPtr<ULevelSequence>(FSoftObjectPath(LevelSequence));
	RuntimeInfo.bIsEntrySequence = false;
	RuntimeInfo.Id = SequenceInfo.Id;

	for (FChronicle_DialogueNodeData Node : SequenceData.Nodes)
	{
		if (Node.Type != EChronicle_DialogueNodeType::Root)
		{
			continue;
		}

		RuntimeInfo.bIsEntrySequence = true;
		break;
	}

	if (SequenceData.NextNodeId != FGuid())
	{
		FChronicle_TransitionInfo Transition;

		for (FChronicle_SequenceData OtherData : CinematicData->SequencesData)
		{
			const FChronicle_DialogueNodeData* MatchingNode = SequenceData.Nodes.FindByPredicate([OtherData](const FChronicle_DialogueNodeData& Node)
			{
				return OtherData.Id == Node.Id;
			});

			if (!MatchingNode)
			{
				continue;
			}
			
			Transition.Type = EChronicle_TransitionType::AutoContinue;
			Transition.NodeId = SequenceData.NextNodeId;
			Transition.Rules = MatchingNode->Rules;
			Transition.SequenceId = OtherData.Id;
			break;
		}
		
		RuntimeInfo.Transitions.Add(Transition);
	}

	for (FGuid SequenceId : SequenceData.BranchSequenceIds)
	{
		for (FChronicle_SequenceData OtherData : CinematicData->SequencesData)
		{
			if (SequenceId != OtherData.Id)
			{
				continue;
			}
			
			FChronicle_TransitionInfo Transition;

			Transition.Type = EChronicle_TransitionType::Response;
			Transition.Rules = OtherData.Nodes[0].Rules;
			Transition.SequenceId = SequenceId;
			
			RuntimeInfo.Transitions.Add(Transition);
		}
	}
	
	return RuntimeInfo;
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
		TrackInfo.Id = Node.Id;

		SequenceInfo.Tracks.Add(TrackInfo);

		FrameCounter += FrameDuration;
	}

	SequenceInfo.TotalFrameCount = FrameCounter;
	SequenceInfo.Id = SequenceData.Id;
	
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
