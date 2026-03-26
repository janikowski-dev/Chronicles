#pragma once

#include "Data/UChronicle_CinematicData.h"

struct FTrackInfo
{
	TSoftObjectPtr<USoundBase> Sound;
	FFrameNumber StartFrame;
	FFrameNumber EndFrame;
	FGuid ParticipantId;
};

struct FSequenceInfo
{
	TMap<FGuid, FTransform> TransformByParticipantIds;
	TMap<FGuid, FGuid> CameraIdByParticipantIds;
	TMap<FGuid, FGuid> ModelIdByParticipantIds;
	FFrameNumber TotalFrameCount;
	TArray<FTrackInfo> Tracks;
};

class FChronicle_CinematicBlueprintUtilities
{
public:
	static TSoftObjectPtr<UWorld> ToWorldPointer(const FString Path);
	
	static UBlueprint* CreateBlueprintFromParent(
		UClass* ParentClass,
		const FString& PackagePath,
		const FString& BlueprintName,
		const UChronicle_CinematicData* Data
	);

	static void InitSequence(
		ULevelSequence* LevelSequence,
		const UChronicle_CinematicData* CinematicData,
		const FChronicle_SequenceData& SequenceData
	);

private:
	static bool TryGetMovieScene(const ULevelSequence* LevelSequence, UMovieScene*& MovieScene);
	static void ApplyInfo(UMovieScene* MovieScene, const FSequenceInfo& SequenceInfo);
	
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
	
	static void ApplyChanges(const ULevelSequence* LevelSequence);
};
