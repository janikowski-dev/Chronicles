#include "FChronicle_RuleEditor.h"

#include "FChronicle_CharacterDirectory.h"
#include "FChronicle_RuleDirectory.h"
#include "GraphEditor.h"
#include "Framework/Commands/GenericCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"
#include "Nodes/Unreal/UChronicle_DialogueNode.h"
#include "Graphs/UChronicle_RuleGraph.h"

static const FName NodeGraphTabName(TEXT("NodeGraph"));

FText FChronicle_RuleEditor::GetBaseToolkitName() const
{
	return FText::FromString("Node Asset Editor");
}

FName FChronicle_RuleEditor::GetToolkitFName() const
{
	return TEXT("Node Asset Editor");
}

FString FChronicle_RuleEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("Node");
}

FLinearColor FChronicle_RuleEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.1f, 0.6f, 1.0f);
}

FText FChronicle_RuleEditor::GetToolkitName() const
{
	return EditedNode->GetTitle();
}

void FChronicle_RuleEditor::InitNodeAssetEditor(
	const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UChronicle_DialogueNode* Node
)
{
	FChronicle_CharacterDirectory::Refresh();
	FChronicle_RuleDirectory::Refresh();
	
	const TSharedRef<FTabManager::FLayout> Layout =
		FTabManager::NewLayout("NodeAssetEditor")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(NodeGraphTabName, ETabState::OpenedTab)
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
		Cache(Node)
	);

	RegenerateMenusAndToolbars();
}

void FChronicle_RuleEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->RegisterTabSpawner(
		NodeGraphTabName,
		FOnSpawnTab::CreateRaw(this, &FChronicle_RuleEditor::SpawnGraphTab)
	)
	.SetDisplayName(FText::FromString("Node Graph"));
}

void FChronicle_RuleEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(NodeGraphTabName);
}

TSharedRef<SDockTab> FChronicle_RuleEditor::SpawnGraphTab(const FSpawnTabArgs&)
{
	UChronicle_RuleGraph* Graph = EditedNode->GetOrCreateInnerGraph();
	
	GraphEditorCommands = MakeShareable(new FUICommandList);

	GraphEditorCommands->MapAction(
		FGenericCommands::Get().Delete,
		FExecuteAction::CreateSP(this, &FChronicle_RuleEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateSP(this, &FChronicle_RuleEditor::CanDeleteNodes)
	);
	
	GraphEditor = SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.GraphToEdit(Graph)
		.IsEditable(true);
	
	return SNew(SDockTab)
		.Label(FText::FromString("Node Graph"))
		[
			GraphEditor.ToSharedRef()
		];
}

void FChronicle_RuleEditor::DeleteSelectedNodes() const
{
	const FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();

	if (SelectedNodes.Num() == 0)
	{
		return;
	}

	GraphEditor->GetCurrentGraph()->Modify();

	for (UObject* Obj : SelectedNodes)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(Obj))
		{
			Node->Modify();
			Node->DestroyNode();
		}
	}

	GraphEditor->ClearSelectionSet();
}

bool FChronicle_RuleEditor::CanDeleteNodes() const
{
	if (!GraphEditor.IsValid())
	{
		return false;
	}

	for (UObject* Obj : GraphEditor->GetSelectedNodes())
	{
		const UEdGraphNode* Node = Cast<UEdGraphNode>(Obj);
		
		if (!Node)
		{
			continue;
		}

		if (!Node->CanUserDeleteNode())
		{
			return false;
		}
	}

	return true;
}

TArray<UObject*> FChronicle_RuleEditor::Cache(UChronicle_DialogueNode* Node)
{
	TArray<UObject*> Result;
	EditedNode = Node;
	Result.Add(Node);
	return Result;
}