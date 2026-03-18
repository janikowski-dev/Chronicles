#pragma once

#include "UChronicle_DialogueData.h"
#include "Data/FChronicle_CinematicEntry.h"
#include "Data/UChronicle_CinematicData.h"
#include "UChronicle_CinematicFunctionLibrary.generated.h"

UCLASS()
class CINEMATICTIMELINEEDITOR_API UChronicle_CinematicFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Chronicle|Dialogue")
	static UChronicle_CinematicData* ExportToCinematicData(const UChronicle_DialogueData* Asset, const FString Path);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Dialogue")
	static UChronicle_CinematicData* ConvertToCinematicData(const UChronicle_DialogueData* Asset);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Dialogue")
	static void UpdateStatus(FString Path, EChronicle_CinematicAssetExportStatus Status);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Dialogue")
	static void UpdateExportInfo(FString Path, const FDateTime ExportTime, const FString& CinematicPath);

	UFUNCTION(BlueprintCallable, Category="Chronicle|Dialogue")
	static void Remove(FString Path);

	UFUNCTION(BlueprintCallable, Category="Chronicle|Dialogue")
	static bool AreEqual(const UChronicle_CinematicData* A, const UChronicle_CinematicData* B);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Dialogue")
	static FChronicle_CinematicEntry TryAdd(FChronicle_CinematicEntry Entry);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Dialogue")
	static TArray<FChronicle_CinematicEntry> GetAll();
};