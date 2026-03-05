#include "FDialogueExporter.h"

#include "JsonObjectConverter.h"
#include "Assets/UDialogueAsset.h"
#include "Graphs/UDialogueGraph.h"
#include "Graphs/URuleGraph.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Nodes/Unreal/UDialogueLinkNode.h"
#include "Nodes/Unreal/UDialogueLineNode.h"
#include "Nodes/Unreal/UDialogueRootNode.h"
#include "Nodes/Unreal/UDialogueResponseNode.h"
#include "Nodes/Unreal/URuleAndNode.h"
#include "Nodes/Unreal/URuleCallbackNode.h"
#include "Nodes/Unreal/URuleConditionNode.h"
#include "Nodes/Unreal/URuleNotNode.h"
#include "Nodes/Unreal/URuleOrNode.h"
#include "Nodes/Unreal/URuleOutputNode.h"
#include "UDialogueData.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

void FDialogueExporter::CopyToClipboard(const UDialogueAsset* Asset)
{
    FString JsonString;
    FJsonObjectConverter::UStructToJsonObjectString(UDialogueData::StaticClass(), ConvertToTemporaryAsset(Asset), JsonString);
    FPlatformApplicationMisc::ClipboardCopy(*JsonString);
    UE_LOG(LogTemp, Log, TEXT("Copied dialogue to clipboard!"));
}

void FDialogueExporter::ExportToAsset(const UDialogueAsset* Asset)
{
    FAssetRegistryModule::AssetCreated(ConvertToAsset(Asset));
}

UDialogueData* FDialogueExporter::ConvertToAsset(const UDialogueAsset* Asset)
{
    const FString AssetName = FString::Printf(TEXT("%s_Data"), *Asset->GetName());
    UPackage* Package = CreatePackage(*(FPackageName::GetLongPackagePath(Asset->GetOutermost()->GetName()) / AssetName));
    UDialogueData* Data = NewObject<UDialogueData>(Package, *AssetName, RF_Public | RF_Standalone);
    ReadData(Asset, Data);
    return Data;
}

UDialogueData* FDialogueExporter::ConvertToTemporaryAsset(const UDialogueAsset* Asset)
{
    UDialogueData* Data = NewObject<UDialogueData>(GetTransientPackage());
    ReadData(Asset, Data);
    return Data;
}

void FDialogueExporter::ReadData(const UDialogueAsset* Asset, UDialogueData* Data)
{
    for (UEdGraphNode* GraphNode : Asset->Graph->Nodes)
    {
        UDialogueNode* Node = Cast<UDialogueNode>(GraphNode);
        FDialogueNodeData NodeData = ReadNodeData(Node);
        TryReadRootData(Data, GraphNode);
        ReadNodeData(Data, NodeData);
    }
}

FDialogueNodeData FDialogueExporter::ReadNodeData(UDialogueNode* Node)
{
    if (!TryGetLinkNodeTarget(Node))
    {
        return {};
    }
    
    FDialogueNodeData NodeData;
    ReadSharedData(Node, NodeData);
    ReadType(Node,NodeData);
    ReadRoles(Node, NodeData);
    ReadRequirements(Node, NodeData);
    ReadCallbacks(Node, NodeData);
    ReadChildren(Node, NodeData);
    return NodeData;
}

void FDialogueExporter::TryReadRootData(UDialogueData* Data, UEdGraphNode* Node)
{
    if (const UDialogueRootNode* RootNode = Cast<UDialogueRootNode>(Node))
    {
        Data->ParticipantIds = RootNode->GetTypedOuter<UDialogueGraph>()->ParticipantIds;
    }
}

void FDialogueExporter::ReadNodeData(UDialogueData* Data, const FDialogueNodeData& NodeData)
{
    Data->Nodes.Add(NodeData);
}

bool FDialogueExporter::TryGetLinkNodeTarget(UDialogueNode*& Node)
{
    if (const UDialogueLinkNode* LinkNode = Cast<UDialogueLinkNode>(Node))
    {
        Node = LinkNode->GetLinkedNode();
        
        if (!Node)
        {
            return false;
        }
    }
    
    return true;
}

void FDialogueExporter::ReadSharedData(const UDialogueNode* Node, FDialogueNodeData& NodeData)
{
    NodeData.Id = Node->Id;
    NodeData.Text = Node->GetText().ToString();
}

void FDialogueExporter::ReadType(const UDialogueNode* Node, FDialogueNodeData& NodeData)
{
    if (Cast<UDialogueRootNode>(Node))
    {
        NodeData.Type = EDialogueNodeType::Root;
    }
    else if (Cast<UDialogueResponseNode>(Node))
    {
        NodeData.Type = EDialogueNodeType::Response;
    }
    else if (Cast<UDialogueLineNode>(Node))
    {
        NodeData.Type = EDialogueNodeType::Line;
    }
}

void FDialogueExporter::ReadRoles(UDialogueNode* Node, FDialogueNodeData& NodeData)
{
    if (const UDialogueLineNode* LineNode = Cast<UDialogueLineNode>(Node))
    {
        NodeData.ListenerId = LineNode->ListenerId;
        NodeData.SpeakerId = LineNode->SpeakerId;
    }
    
    if (const UDialogueResponseNode* ResponseNode = Cast<UDialogueResponseNode>(Node))
    {
        NodeData.ListenerId = ResponseNode->ListenerId;
    }
}

void FDialogueExporter::ReadRequirements(const UDialogueNode* Node, FDialogueNodeData& NodeData)
{
    if (const URuleGraph* RuleGraph = Node->GetInnerGraph())
    {
        for (URuleNode* Rule : RuleGraph->GetRules(EOutputType::Requirements))
        {
            FRuleData RuleData;

            if (Cast<URuleOutputNode>(Rule))
            {
                RuleData.Type = EConditionNodeType::Output;
            }
            else if (Cast<URuleConditionNode>(Rule))
            {
                RuleData.Type = EConditionNodeType::Raw;
            }
            else if (Cast<URuleAndNode>(Rule))
            {
                RuleData.Type = EConditionNodeType::And;
            }
            else if (Cast<URuleOrNode>(Rule))
            {
                RuleData.Type = EConditionNodeType::Or;
            }
            else if (Cast<URuleNotNode>(Rule))
            {
                RuleData.Type = EConditionNodeType::Not;
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
                    const URuleNode* Input = Cast<URuleNode>(Linked->GetOwningNode());
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
                    const URuleNode* Output = Cast<URuleNode>(Linked->GetOwningNode());
                    RuleData.Output.Add(Output->Id);
                }

                NodeData.Rules.Add(RuleData);
            }
        }
    }
}

void FDialogueExporter::ReadCallbacks(const UDialogueNode* Node, FDialogueNodeData& NodeData)
{
    if (const URuleGraph* RuleGraph = Node->GetInnerGraph())
    {
        for (URuleNode* Rule : RuleGraph->GetRules(EOutputType::PostCallback))
        {
            if (const URuleCallbackNode* Callback = Cast<URuleCallbackNode>(Rule))
            {
                NodeData.Callbacks.Add(Callback->RuleId);
            }
        }
    }
}

void FDialogueExporter::ReadChildren(UDialogueNode* Node, FDialogueNodeData& NodeData)
{
    for (UEdGraphPin* Pin : Node->Pins)
    {
        if (Pin->Direction != EGPD_Output)
        {
            continue;
        }
        
        for (const UEdGraphPin* Linked : Pin->LinkedTo)
        {
            const UDialogueNode* Response = Cast<UDialogueNode>(Linked->GetOwningNode());
            NodeData.Children.Add(Response->Id);
        }
    }
}
