#pragma once

#include "Data/UChronicle_DialogueData.h"
#include "Data/FChronicle_CinematicEntry.h"
#include "Data/FChronicle_DialogueInfo.h"
#include "Data/UChronicle_CinematicData.h"
#include "UChronicle_CinematicFunctionLibrary.generated.h"

UCLASS()
class CINEMATICTIMELINEEDITOR_API UChronicle_CinematicFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Chronicle|Cinematics")
	static UChronicle_CinematicData* ExportToCinematicData(const UChronicle_DialogueData* Asset, const FString Path);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Cinematics")
	static UChronicle_CinematicData* ConvertToCinematicData(const UChronicle_DialogueData* Asset);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Cinematics")
	static FChronicle_DialogueInfo Get(const FGuid& DialogueInfoId);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Cinematics")
	static void Override(const FGuid& DialogueInfoId, const FChronicle_SequenceInfo& SequenceInfo);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Cinematics")
	static void UpdateStatus(FString Path, EChronicle_CinematicAssetExportStatus Status);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Cinematics")
	static void UpdateExportInfo(FString Path, const FDateTime ExportTime, const FString& CinematicPath);

	UFUNCTION(BlueprintCallable, Category="Chronicle|Cinematics")
	static void Remove(FString Path);

	UFUNCTION(BlueprintCallable, Category="Chronicle|Cinematics")
	static bool AreEqual(const UChronicle_CinematicData* A, const UChronicle_CinematicData* B);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Cinematics")
	static FChronicle_CinematicEntry TryAdd(FChronicle_CinematicEntry Entry);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Cinematics")
	static TArray<FChronicle_CinematicEntry> GetAll();
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Cinematics")
	static FChronicle_SequenceInfo InitSequence(
		ULevelSequence* LevelSequence,
		UChronicle_CinematicData* CinematicData,
		const FChronicle_SequenceData& SequenceData
	);
	
	UFUNCTION(BlueprintCallable, Category = "Chronicle|Cinematics")
	static UBlueprint* CreateBlueprintFromParent(
	    UClass* ParentClass,
	    const FString& PackagePath,
	    const FString& BlueprintName,
		const FChronicle_DialogueInfo& Info,
		const FTransform& ResponseTransform,
		const TArray<FTransform>& CameraTransforms,
		const TArray<FTransform>& ParticipantTransforms
	);
};