#include "FDialogueAssetEditor.h"

#include "FChronicleCharacterDirectory.h"
#include "Assets/UDialogueAsset.h"
#include "Graphs/UDialogueGraph.h"
#include "GraphEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"

static const FName DialogueGraphTabName(TEXT("DialogueGraph"));

FName FDialogueAssetEditor::GetToolkitFName() const
{
	return FName("DialogueAssetEditor");
}

FText FDialogueAssetEditor::GetBaseToolkitName() const
{
	return FText::FromString("Dialogue Asset Editor");
}

FString FDialogueAssetEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("Dialogue");
}

FLinearColor FDialogueAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.1f, 0.6f, 1.0f);
}

void FDialogueAssetEditor::InitDialogueAssetEditor(
	const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UDialogueAsset* Asset
)
{
	FChronicleCharacterDirectory::Refresh();
	
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

void FDialogueAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->RegisterTabSpawner(
		DialogueGraphTabName,
		FOnSpawnTab::CreateRaw(this, &FDialogueAssetEditor::SpawnGraphTab)
	)
	.SetDisplayName(FText::FromString("Dialogue Graph"));
}

void FDialogueAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(DialogueGraphTabName);
}

TSharedRef<SDockTab> FDialogueAssetEditor::SpawnGraphTab(const FSpawnTabArgs&)
{
	const TObjectPtr<UDialogueGraph> Graph = EditedAsset->Graph;
	
	GraphEditor = SNew(SGraphEditor)
		.GraphToEdit(Graph)
		.IsEditable(true);
	
	return SNew(SDockTab)
		.Label(FText::FromString("Dialogue Graph"))
		[
			GraphEditor.ToSharedRef()
		];
}

TArray<UObject*> FDialogueAssetEditor::Cache(UDialogueAsset* Asset)
{
	TArray<UObject*> Result;
	EditedAsset = Asset;
	Result.Add(Asset);
	return Result;
}