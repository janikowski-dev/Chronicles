#pragma once

#include "FDialogueNodeData.h"
#include "Nodes/Unreal/UDialogueRootNode.h"

class UDialogueData;
class UDialogueNode;
class URuleNode;
class UDialogueAsset;

class FDialogueExporter
{
public:
	static void CopyToClipboard(const UDialogueAsset* Asset);
	static void ExportToAsset(const UDialogueAsset* Asset);

private:
	static UDialogueData* ConvertToAsset(const UDialogueAsset* Asset);
	static UDialogueData* ConvertToTemporaryAsset(const UDialogueAsset* Asset);
	
	static void ReadData(const UDialogueAsset* Asset, UDialogueData* Data);
	static FDialogueNodeData ReadNodeData(UDialogueNode* Node);
	
	static void TryReadRootData(UDialogueData* Data, UEdGraphNode* Node);
	static void ReadNodeData(UDialogueData* Data, const FDialogueNodeData& NodeData);
	
	static bool TryGetLinkNodeTarget(UDialogueNode*& Node);
	static void ReadSharedData(const UDialogueNode* Node, FDialogueNodeData& NodeData);
	static void ReadType(const UDialogueNode* Node, FDialogueNodeData& NodeData);
	static void ReadRoles(UDialogueNode* Node, FDialogueNodeData& NodeData);
	static void ReadRequirements(const UDialogueNode* Node, FDialogueNodeData& NodeData);
	static void ReadCallbacks(const UDialogueNode* Node, FDialogueNodeData& NodeData);
	static void ReadChildren(UDialogueNode* Node, FDialogueNodeData& NodeData);
};