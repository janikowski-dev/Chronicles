#pragma once

#include "FChronicle_DialogueNodeData.h"
#include "Nodes/Unreal/UChronicle_DialogueRootNode.h"

class UChronicle_DialogueData;
class UChronicle_DialogueNode;
class UChronicle_RuleNode;
class UChronicle_DialogueAsset;

class DIALOGUESYSTEMEDITOR_API FChronicle_DialogueExporter
{
public:
	static UChronicle_DialogueData* ConvertToData(const UChronicle_DialogueAsset* Asset);
	static FString ConvertToJson(const UChronicle_DialogueAsset* Asset);
	
	static void ExportJsonToClipboard(const UChronicle_DialogueAsset* Asset);
	static void ExportToData(const UChronicle_DialogueAsset* Asset);

private:
	static UChronicle_DialogueData* ConvertToData_Internal(const UChronicle_DialogueAsset* Asset);
	static FString ConvertToJson_Internal(const UChronicle_DialogueAsset* Asset);
	
	static UChronicle_DialogueData* ConvertToTemporaryData(const UChronicle_DialogueAsset* Asset);
	
	static void ReadData(const UChronicle_DialogueAsset* Asset, UChronicle_DialogueData* Data);
	static FChronicle_DialogueNodeData ReadNodeData(UChronicle_DialogueNode* Node);
	
	static void TryReadRootData(UChronicle_DialogueData* Data, UEdGraphNode* Node);
	static void ReadLinkData(UEdGraphNode* Node, FChronicle_DialogueNodeData& NodeData);
	static void ReadNodeData(UChronicle_DialogueData* Data, const FChronicle_DialogueNodeData& NodeData);
	
	static void ReadSharedData(const UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData);
	static void ReadType(const UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData);
	static void ReadRoles(UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData);
	static void ReadRequirements(const UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData);
	static void ReadCallbacks(const UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData);
	static void ReadChildren(UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData);
};