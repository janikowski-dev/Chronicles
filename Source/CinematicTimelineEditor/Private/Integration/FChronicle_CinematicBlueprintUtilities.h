#pragma once

#include "Data/FChronicle_DialogueInfo.h"
#include "Data/FChronicle_SequenceInfo.h"
#include "Data/UChronicle_CinematicData.h"

struct CINEMATICTIMELINEEDITOR_API FTrackInfo
{
	TSoftObjectPtr<USoundBase> Sound;
	FFrameNumber StartFrame;
	FFrameNumber EndFrame;
	FGuid ParticipantId;
	FGuid Id;
};

struct CINEMATICTIMELINEEDITOR_API FSequenceInfo
{
	TMap<FGuid, FTransform> TransformByParticipantIds;
	TMap<FGuid, FGuid> CameraIdByParticipantIds;
	TMap<FGuid, FGuid> ModelIdByParticipantIds;
	FFrameNumber TotalFrameCount;
	TArray<FTrackInfo> Tracks;
	FGuid Id;
};

class CINEMATICTIMELINEEDITOR_API FChronicle_CinematicBlueprintUtilities
{
public:
	static TSoftObjectPtr<UWorld> ToWorldPointer(const FString Path);
	
	static UBlueprint* CreateBlueprintFromParent(
		UClass* ParentClass,
		const FString& PackagePath,
		const FString& BlueprintName,
		const FChronicle_DialogueInfo& Info
	);

	static FChronicle_SequenceInfo InitSequence(
		ULevelSequence* LevelSequence,
		UChronicle_CinematicData* CinematicData,
		const FChronicle_SequenceData& SequenceData
	);

private:
	static bool TryGetMovieScene(const ULevelSequence* LevelSequence, UMovieScene*& MovieScene);
	static void ApplyInfo(UMovieScene* MovieScene, const FSequenceInfo& SequenceInfo);

	static FChronicle_SequenceInfo ConvertToRuntimeInfo(
		const ULevelSequence* LevelSequence,
		const FSequenceInfo& SequenceInfo,
		const UChronicle_CinematicData* CinematicData,
		const FChronicle_SequenceData& SequenceData
	);
	
	static FSequenceInfo ConvertToInfo(
		ULevelSequence* LevelSequence,
		UMovieScene* MovieScene,
		const UChronicle_CinematicData* CinematicData,
		const FChronicle_SequenceData& SequenceData
	);
	
	static void PopulateCameraCutTrack(
		UMovieScene* MovieScene,
		const FSequenceInfo& SequenceInfo
	);
	
	static void PopulateAudioTrack(
		UMovieScene* MovieScene,
		const FSequenceInfo& SequenceInfo
	);
	
	static FGuid AddCamera(
		ULevelSequence* LevelSequence,
		UMovieScene* MovieScene,
		const FTransform& SpawnTransform
	);
	
	static FGuid AddModel(
		ULevelSequence* LevelSequence,
		UMovieScene* MovieScene,
		USkeletalMesh* SkeletalMesh,
		const FTransform& SpawnTransform
	);
	
	static void ApplyChanges(
		ULevelSequence* LevelSequence,
		UChronicle_CinematicData* CinematicData,
		const FChronicle_SequenceData& SequenceData
	);
};
