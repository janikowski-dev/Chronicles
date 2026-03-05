#include "FChronicle_DialogueEditor.h"

#include "FChronicle_CharacterDirectory.h"
#include "Assets/UChronicle_DialogueAsset.h"
#include "Graphs/UChronicle_DialogueGraph.h"
#include "GraphEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"

static const FName DialogueGraphTabName(TEXT("DialogueGraph"));

FName FChronicle_DialogueEditor::GetToolkitFName() const
{
	return FName("DialogueAssetEditor");
}

FText FChronicle_DialogueEditor::GetBaseToolkitName() const
{
	return FText::FromString("Dialogue Asset Editor");
}

FString FChronicle_DialogueEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("Dialogue");
}

FLinearColor FChronicle_DialogueEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.1f, 0.6f, 1.0f);
}

void FChronicle_DialogueEditor::InitDialogueAssetEditor(
	const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UChronicle_DialogueAsset* Asset
)
{
	FChronicle_CharacterDirectory::Refresh();
	
	const TSharedRef<FTabManager::FLayout> Layout =
		FTabManager::NewLayout("DialogueAssetEditor")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(DialogueGraphTabName, ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
		);

	InitAssetEditor(
		Mode,
		InitToolkitHost,
		GetToolkitFName(),
		Layout,
		true,
		true,
		Cache(Asset)
	);

	RegenerateMenusAndToolbars();
}

void FChronicle_DialogueEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->RegisterTabSpawner(
		DialogueGraphTabName,
		FOnSpawnTab::CreateRaw(this, &FChronicle_DialogueEditor::SpawnGraphTab)
	)
	.SetDisplayName(FText::FromString("Dialogue Graph"));
}

void FChronicle_DialogueEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(DialogueGraphTabName);
}

TSharedRef<SDockTab> FChronicle_DialogueEditor::SpawnGraphTab(const FSpawnTabArgs&)
{
	const TObjectPtr<UChronicle_DialogueGraph> Graph = EditedAsset->Graph;
	
	GraphEditor = SNew(SGraphEditor)
		.GraphToEdit(Graph)
		.IsEditable(true);
	
	return SNew(SDockTab)
		.Label(FText::FromString("Dialogue Graph"))
		[
			GraphEditor.ToSharedRef()
		];
}

TArray<UObject*> FChronicle_DialogueEditor::Cache(UChronicle_DialogueAsset* Asset)
{
	TArray<UObject*> Result;
	EditedAsset = Asset;
	Result.Add(Asset);
	return Result;
}