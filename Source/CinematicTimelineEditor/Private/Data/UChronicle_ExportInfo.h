#pragma once

#include "FChronicle_CinematicEntry.h"
#include "UChronicle_CinematicData.h"
#include "Data/FChronicle_DialogueInfo.h"
#include "Engine/DeveloperSettings.h"
#include "UChronicle_ExportInfo.generated.h"

UCLASS(Config=CinematicTimelineEditor, DefaultConfig, meta=(DisplayName="Export Info"))
class CINEMATICTIMELINEEDITOR_API UChronicle_ExportInfo : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	virtual FName GetCategoryName() const override;

	void Override(const FGuid& DialogueInfoId, const FChronicle_SequenceInfo& SequenceInfo);
	FChronicle_DialogueInfo Get(const FGuid& DialogueInfoId);
	
	void UpdateStatus(const FString& Path, const EChronicle_CinematicAssetExportStatus Status);
	void UpdateExportInfo(const FString& Path, const FDateTime ExportTime, const FString& CinematicPath);
	FChronicle_CinematicEntry TryAdd(const FChronicle_CinematicEntry& InEntry);
	void Remove(const FString& Path);
	bool AreEqual(const UChronicle_CinematicData* A, const UChronicle_CinematicData* B);

public:
	UPROPERTY(Config, EditAnywhere, meta=(DisplayPriority=0))
	TArray<FChronicle_CinematicEntry> Entries;
	
	UPROPERTY(Config, EditAnywhere, meta=(DisplayPriority=1))
	TArray<FChronicle_DialogueInfo> Infos;
};