#include "UChronicle_CinematicFunctionLibrary.h"

#include "FChronicle_CinematicBlueprintUtilities.h"
#include "FChronicle_CinematicExporter.h"
#include "Data/UChronicle_ExportInfo.h"

UChronicle_CinematicData* UChronicle_CinematicFunctionLibrary::ExportToCinematicData(const UChronicle_DialogueData* Asset, const FString Path)
{
	return FChronicle_CinematicExporter::ExportToCinematicData(Asset, Path);
}

UChronicle_CinematicData* UChronicle_CinematicFunctionLibrary::ConvertToCinematicData(const UChronicle_DialogueData* Asset)
{
	return FChronicle_CinematicExporter::ConvertToCinematicData(Asset);
}

FChronicle_DialogueInfo UChronicle_CinematicFunctionLibrary::Get(const FGuid& DialogueInfoId)
{
	return GetMutableDefault<UChronicle_ExportInfo>()->Get(DialogueInfoId);
}

void UChronicle_CinematicFunctionLibrary::Override(const FGuid& DialogueInfoId, const FChronicle_SequenceInfo& SequenceInfo)
{
	GetMutableDefault<UChronicle_ExportInfo>()->Override(DialogueInfoId, SequenceInfo);
	GetMutableDefault<UChronicle_ExportInfo>()->SaveConfig();
}

void UChronicle_CinematicFunctionLibrary::UpdateStatus(const FString Path, const EChronicle_CinematicAssetExportStatus Status)
{
	GetMutableDefault<UChronicle_ExportInfo>()->UpdateStatus(Path, Status);
	GetMutableDefault<UChronicle_ExportInfo>()->SaveConfig();
}

void UChronicle_CinematicFunctionLibrary::UpdateExportInfo(const FString Path, const FDateTime ExportTime, const FString& CinematicPath)
{
	GetMutableDefault<UChronicle_ExportInfo>()->UpdateExportInfo(Path, ExportTime, CinematicPath);
	GetMutableDefault<UChronicle_ExportInfo>()->SaveConfig();
}

void UChronicle_CinematicFunctionLibrary::Remove(const FString Path)
{
	GetMutableDefault<UChronicle_ExportInfo>()->Remove(Path);
	GetMutableDefault<UChronicle_ExportInfo>()->SaveConfig();
}

bool UChronicle_CinematicFunctionLibrary::AreEqual(const UChronicle_CinematicData* A, const UChronicle_CinematicData* B)
{
	return GetMutableDefault<UChronicle_ExportInfo>()->AreEqual(A, B);
}

FChronicle_CinematicEntry UChronicle_CinematicFunctionLibrary::TryAdd(FChronicle_CinematicEntry Entry)
{
	FChronicle_CinematicEntry NewOrExisting = GetMutableDefault<UChronicle_ExportInfo>()->TryAdd(Entry);
	GetMutableDefault<UChronicle_ExportInfo>()->SaveConfig();
	return NewOrExisting;
}

TArray<FChronicle_CinematicEntry> UChronicle_CinematicFunctionLibrary::GetAll()
{
	return GetDefault<UChronicle_ExportInfo>()->Entries;
}

TSoftObjectPtr<UWorld> UChronicle_CinematicFunctionLibrary::ToWorldPointer(const FString Path)
{
	return FChronicle_CinematicBlueprintUtilities::ToWorldPointer(Path);
}

FChronicle_SequenceInfo UChronicle_CinematicFunctionLibrary::InitSequence(
	ULevelSequence* LevelSequence,
	UChronicle_CinematicData* CinematicData,
	const FChronicle_SequenceData& SequenceData
)
{
	return FChronicle_CinematicBlueprintUtilities::InitSequence(LevelSequence, CinematicData, SequenceData);
}

UBlueprint* UChronicle_CinematicFunctionLibrary::CreateBlueprintFromParent(
	UClass* ParentClass,
	const FString& PackagePath,
	const FString& BlueprintName,
	const FChronicle_DialogueInfo& Info
)
{
	return FChronicle_CinematicBlueprintUtilities::CreateBlueprintFromParent(ParentClass, PackagePath, BlueprintName, Info);
}
