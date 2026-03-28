#pragma once

UENUM(BlueprintType)
enum class EChronicle_CinematicAssetExportStatus : uint8
{
	NotExported = 0,
	NotUpToDate = 1,
	UpToDate = 2
};