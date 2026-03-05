#include "FChronicle_DialogueExporter.h"

#include "JsonObjectConverter.h"
#include "Assets/UChronicle_DialogueAsset.h"
#include "Graphs/UChronicle_DialogueGraph.h"
#include "Graphs/UChronicle_RuleGraph.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Nodes/Unreal/UChronicle_DialogueLinkNode.h"
#include "Nodes/Unreal/UChronicle_DialogueLineNode.h"
#include "Nodes/Unreal/UChronicle_DialogueRootNode.h"
#include "Nodes/Unreal/UChronicle_DialogueResponseNode.h"
#include "Nodes/Unreal/UChronicle_RuleAndNode.h"
#include "Nodes/Unreal/UChronicle_RuleCallbackNode.h"
#include "Nodes/Unreal/UChronicle_RuleConditionNode.h"
#include "Nodes/Unreal/UChronicle_RuleNotNode.h"
#include "Nodes/Unreal/UChronicle_RuleOrNode.h"
#include "Nodes/Unreal/UChronicle_RuleOutputNode.h"
#include "UChronicle_DialogueData.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

void FChronicle_DialogueExporter::CopyToClipboard(const UChronicle_DialogueAsset* Asset)
{
    FString JsonString;
    FJsonObjectConverter::UStructToJsonObjectString(UChronicle_DialogueData::StaticClass(), ConvertToTemporaryAsset(Asset), JsonString);
    FPlatformApplicationMisc::ClipboardCopy(*JsonString);
    UE_LOG(LogTemp, Log, TEXT("Copied dialogue to clipboard!"));
}

void FChronicle_DialogueExporter::ExportToAsset(const UChronicle_DialogueAsset* Asset)
{
    FAssetRegistryModule::AssetCreated(ConvertToAsset(Asset));
}

UChronicle_DialogueData* FChronicle_DialogueExporter::ConvertToAsset(const UChronicle_DialogueAsset* Asset)
{
    const FString AssetName = FString::Printf(TEXT("%s_Data"), *Asset->GetName());
    UPackage* Package = CreatePackage(*(FPackageName::GetLongPackagePath(Asset->GetOutermost()->GetName()) / AssetName));
    UChronicle_DialogueData* Data = NewObject<UChronicle_DialogueData>(Package, *AssetName, RF_Public | RF_Standalone);
    ReadData(Asset, Data);
    return Data;
}

UChronicle_DialogueData* FChronicle_DialogueExporter::ConvertToTemporaryAsset(const UChronicle_DialogueAsset* Asset)
{
    UChronicle_DialogueData* Data = NewObject<UChronicle_DialogueData>(GetTransientPackage());
    ReadData(Asset, Data);
    return Data;
}

void FChronicle_DialogueExporter::ReadData(const UChronicle_DialogueAsset* Asset, UChronicle_DialogueData* Data)
{
    for (UEdGraphNode* GraphNode : Asset->Graph->Nodes)
    {
        UChronicle_DialogueNode* Node = Cast<UChronicle_DialogueNode>(GraphNode);
        FChronicle_DialogueNodeData NodeData = ReadNodeData(Node);
        TryReadRootData(Data, GraphNode);
        ReadNodeData(Data, NodeData);
    }
}

FChronicle_DialogueNodeData FChronicle_DialogueExporter::ReadNodeData(UChronicle_DialogueNode* Node)
{
    if (!TryGetLinkNodeTarget(Node))
    {
        return {};
    }
    
    FChronicle_DialogueNodeData NodeData;
    ReadSharedData(Node, NodeData);
    ReadType(Node,NodeData);
    ReadRoles(Node, NodeData);
    ReadRequirements(Node, NodeData);
    ReadCallbacks(Node, NodeData);
    ReadChildren(Node, NodeData);
    return NodeData;
}

void FChronicle_DialogueExporter::TryReadRootData(UChronicle_DialogueData* Data, UEdGraphNode* Node)
{
    if (const UChronicle_DialogueRootNode* RootNode = Cast<UChronicle_DialogueRootNode>(Node))
    {
        Data->ParticipantIds = RootNode->GetTypedOuter<UChronicle_DialogueGraph>()->ParticipantIds;
    }
}

void FChronicle_DialogueExporter::ReadNodeData(UChronicle_DialogueData* Data, const FChronicle_DialogueNodeData& NodeData)
{
    Data->Nodes.Add(NodeData);
}

bool FChronicle_DialogueExporter::TryGetLinkNodeTarget(UChronicle_DialogueNode*& Node)
{
    if (const UChronicle_DialogueLinkNode* LinkNode = Cast<UChronicle_DialogueLinkNode>(Node))
    {
        Node = LinkNode->GetLinkedNode();
        
        if (!Node)
        {
            return false;
        }
    }
    
    return true;
}

void FChronicle_DialogueExporter::ReadSharedData(const UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData)
{
    NodeData.Id = Node->Id;
    NodeData.Text = Node->GetText().ToString();
}

void FChronicle_DialogueExporter::ReadType(const UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData)
{
    if (Cast<UChronicle_DialogueRootNode>(Node))
    {
        NodeData.Type = EChronicle_DialogueNodeType::Root;
    }
    else if (Cast<UChronicle_DialogueResponseNode>(Node))
    {
        NodeData.Type = EChronicle_DialogueNodeType::Response;
    }
    else if (Cast<UChronicle_DialogueLineNode>(Node))
    {
        NodeData.Type = EChronicle_DialogueNodeType::Line;
    }
}

void FChronicle_DialogueExporter::ReadRoles(UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData)
{
    if (const UChronicle_DialogueLineNode* LineNode = Cast<UChronicle_DialogueLineNode>(Node))
    {
        NodeData.ListenerId = LineNode->ListenerId;
        NodeData.SpeakerId = LineNode->SpeakerId;
    }
    
    if (const UChronicle_DialogueResponseNode* ResponseNode = Cast<UChronicle_DialogueResponseNode>(Node))
    {
        NodeData.ListenerId = ResponseNode->ListenerId;
    }
}

void FChronicle_DialogueExporter::ReadRequirements(const UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData)
{
    if (const UChronicle_RuleGraph* RuleGraph = Node->GetInnerGraph())
    {
        for (UChronicle_RuleNode* Rule : RuleGraph->GetRules(EOutputType::Requirements))
        {
            FChronicle_RuleData RuleData;

            if (Cast<UChronicle_RuleOutputNode>(Rule))
            {
                RuleData.Type = EChronicle_ConditionNodeType::Output;
            }
            else if (Cast<UChronicle_RuleConditionNode>(Rule))
            {
                RuleData.Type = EChronicle_ConditionNodeType::Raw;
            }
            else if (Cast<UChronicle_RuleAndNode>(Rule))
            {
                RuleData.Type = EChronicle_ConditionNodeType::And;
            }
            else if (Cast<UChronicle_RuleOrNode>(Rule))
            {
                RuleData.Type = EChronicle_ConditionNodeType::Or;
            }
            else if (Cast<UChronicle_RuleNotNode>(Rule))
            {
                RuleData.Type = EChronicle_ConditionNodeType::Not;
            }
            else
            {
                continue;
            }

            RuleData.Id = Rule->Id;

            for (UEdGraphPin* Pin : Rule->Pins)
            {
                if (Pin->Direction != EGPD_Input)
                {
                    continue;
                }

                for (const UEdGraphPin* Linked : Pin->LinkedTo)
                {
                    const UChronicle_RuleNode* Input = Cast<UChronicle_RuleNode>(Linked->GetOwningNode());
                    RuleData.Input.Add(Input->Id);
                }

                NodeData.Rules.Add(RuleData);
            }
            
            for (UEdGraphPin* Pin : Rule->Pins)
            {
                if (Pin->Direction != EGPD_Output)
                {
                    continue;
                }
                
                for (const UEdGraphPin* Linked : Pin->LinkedTo)
                {
                    const UChronicle_RuleNode* Output = Cast<UChronicle_RuleNode>(Linked->GetOwningNode());
                    RuleData.Output.Add(Output->Id);
                }

                NodeData.Rules.Add(RuleData);
            }
        }
    }
}

void FChronicle_DialogueExporter::ReadCallbacks(const UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData)
{
    if (const UChronicle_RuleGraph* RuleGraph = Node->GetInnerGraph())
    {
        for (UChronicle_RuleNode* Rule : RuleGraph->GetRules(EOutputType::PostCallback))
        {
            if (const UChronicle_RuleCallbackNode* Callback = Cast<UChronicle_RuleCallbackNode>(Rule))
            {
                NodeData.Callbacks.Add(Callback->RuleId);
            }
        }
    }
}

void FChronicle_DialogueExporter::ReadChildren(UChronicle_DialogueNode* Node, FChronicle_DialogueNodeData& NodeData)
{
    for (UEdGraphPin* Pin : Node->Pins)
    {
        if (Pin->Direction != EGPD_Output)
        {
            continue;
        }
        
        for (const UEdGraphPin* Linked : Pin->LinkedTo)
        {
            const UChronicle_DialogueNode* Response = Cast<UChronicle_DialogueNode>(Linked->GetOwningNode());
            NodeData.Children.Add(Response->Id);
        }
    }
}
