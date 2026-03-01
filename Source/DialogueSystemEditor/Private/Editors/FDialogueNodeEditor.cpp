#include "FDialogueNodeEditor.h"

#include "GraphEditor.h"
#include "Framework/Commands/GenericCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"
#include "Nodes/Unreal/UDialogueNode.h"
#include "Graphs/URuleGraph.h"

static const FName NodeGraphTabName(TEXT("NodeGraph"));

FText FDialogueNodeEditor::GetBaseToolkitName() const
{
	return FText::FromString("Node Asset Editor");
}

FName FDialogueNodeEditor::GetToolkitFName() const
{
	return TEXT("Node Asset Editor");
}

FString FDialogueNodeEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("Node");
}

FLinearColor FDialogueNodeEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.1f, 0.6f, 1.0f);
}

FText FDialogueNodeEditor::GetToolkitName() const
{
	return EditedNode->GetTitle();
}

void FDialogueNodeEditor::InitNodeAssetEditor(
	const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UDialogueNode* Node
)
{
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

void FDialogueNodeEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->RegisterTabSpawner(
		NodeGraphTabName,
		FOnSpawnTab::CreateRaw(this, &FDialogueNodeEditor::SpawnGraphTab)
	)
	.SetDisplayName(FText::FromString("Node Graph"));
}

void FDialogueNodeEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(NodeGraphTabName);
}

TSharedRef<SDockTab> FDialogueNodeEditor::SpawnGraphTab(const FSpawnTabArgs&)
{
	URuleGraph* Graph = EditedNode->GetOrCreateInnerGraph();
	
	GraphEditorCommands = MakeShareable(new FUICommandList);

	GraphEditorCommands->MapAction(
		FGenericCommands::Get().Delete,
		FExecuteAction::CreateSP(this, &FDialogueNodeEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateSP(this, &FDialogueNodeEditor::CanDeleteNodes)
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

void FDialogueNodeEditor::DeleteSelectedNodes() const
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

bool FDialogueNodeEditor::CanDeleteNodes() const
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

TArray<UObject*> FDialogueNodeEditor::Cache(UDialogueNode* Node)
{
	TArray<UObject*> Result;
	EditedNode = Node;
	Result.Add(Node);
	return Result;
}