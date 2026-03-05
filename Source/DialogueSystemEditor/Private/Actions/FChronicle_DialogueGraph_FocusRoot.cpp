#include "FChronicle_DialogueGraph_FocusRoot.h"

#include "Editors/FChronicle_DialogueEditor.h"
#include "Nodes/Unreal/UChronicle_DialogueNode.h"
#include "Graphs/UChronicle_DialogueGraph.h"

UEdGraphNode* FChronicle_DialogueGraph_FocusRoot::PerformAction(
	UEdGraph* ParentGraph,
	UEdGraphPin*,
	const FVector2f&,
	bool
)
{
	const UChronicle_DialogueNode* RootNode = Cast<UChronicle_DialogueGraph>(ParentGraph)->GetRootNode();
	
	if (!RootNode)
	{
		return nullptr;
	}

	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	IAssetEditorInstance* EditorInstance = AssetEditorSubsystem->FindEditorForAsset(ParentGraph->GetOuter(), false);
    
	if (const FChronicle_DialogueEditor* DialogueEditor = static_cast<FChronicle_DialogueEditor*>(EditorInstance))
	{
		DialogueEditor->GraphEditor->JumpToNode(Cast<UEdGraphNode>(RootNode));
	}
	
	return nullptr;
}
