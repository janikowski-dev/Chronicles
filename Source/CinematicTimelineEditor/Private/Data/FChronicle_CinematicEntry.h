#pragma once

#include "EChronicle_CinematicAssetExportStatus.h"
#include "FChronicle_CinematicEntry.generated.h"

USTRUCT(BlueprintType)
struct CINEMATICTIMELINEEDITOR_API FChronicle_CinematicEntry
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString AssetName;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString MainSubPath;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString MainPath;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString CinematicPath;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FDateTime LastExportTime;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	EChronicle_CinematicAssetExportStatus Status;
};