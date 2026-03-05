#include "SChronicle_DialogueLinkNode.h"

#include "FChronicle_CharacterDirectory.h"
#include "Graphs/UChronicle_DialogueGraph.h"
#include "Nodes/Unreal/UChronicle_DialogueLineNode.h"
#include "Nodes/Unreal/UChronicle_DialogueLinkNode.h"
#include "Utils/FChronicle_Colors.h"
#include "Utils/FChronicle_EditorStyle.h"
#include "Utils/FChronicle_SlateHelper.h"

void SChronicle_DialogueLinkNode::Construct(const FArguments&, UChronicle_DialogueLinkNode* InNode)
{
    Cache(InNode);
    UpdateGraphNode();
}

FSlateColor SChronicle_DialogueLinkNode::GetHeaderColor() const
{
    return FChronicle_Colors::Link;
}

void SChronicle_DialogueLinkNode::AddBody(const TSharedRef<SVerticalBox>& Box)
{
    Box->AddSlot()
    .AutoHeight()
    .Padding(4)
    [
        SNew(SComboButton)
        .OnGetMenuContent(this, &SChronicle_DialogueLinkNode::GetLineNodesMenu)
        .ButtonContent()
        [
            SNew(SBox)
            .Padding(FMargin(0.0f, 4.0f))
            [
                SNew(STextBlock)
                .Text(this, &SChronicle_DialogueLinkNode::GetSelectedNodeTitle)
            ]
        ]
    ];

    if (!TypedNode->GetLinkedNode())
    {
        return;
    }

    Box->AddSlot()
    .AutoHeight()
    .Padding(4)
    [
        FChronicle_SlateHelper::MakeCharacterDisplay(
            FChronicle_EditorStyle::Get().GetBrush("Icons.Speaker"),
            TAttribute<FText>(this, &SChronicle_DialogueLinkNode::GetSpeakerName)
        )
    ];

    Box->AddSlot()
    .AutoHeight()
    .Padding(4)
    [
        FChronicle_SlateHelper::MakeCharacterDisplay(
            FChronicle_EditorStyle::Get().GetBrush("Icons.Listener"),
            TAttribute<FText>(this, &SChronicle_DialogueLinkNode::GetListenerName)
        )
    ];
    
    Box->AddSlot()
    .AutoHeight()
    [
        FChronicle_SlateHelper::MakeTextField(TAttribute<FText>(this, &SChronicle_DialogueLinkNode::GetSelectedNodeText))
    ];
}

TSharedRef<SWidget> SChronicle_DialogueLinkNode::GetLineNodesMenu()
{
    FMenuBuilder MenuBuilder(true, nullptr);

    for (TWeakObjectPtr Node : TypedGraph->LineNodes)
    {
        if (!Node.IsValid())
        {
            continue;
        }

        UChronicle_DialogueLineNode* LineNode = Node.Get();

        MenuBuilder.AddMenuEntry(
            LineNode->GetTitle(),
            FText::FromString("Link to this dialogue line"),
            FSlateIcon(),
            FUIAction(FExecuteAction::CreateSP(this, &SChronicle_DialogueLinkNode::SelectNode, LineNode))
        );
    }

    return MenuBuilder.MakeWidget();
}

FText SChronicle_DialogueLinkNode::GetSelectedNodeTitle() const
{
    if (TypedNode.IsValid() && TypedNode->GetLinkedNode())
    {
        return TypedNode->GetLinkedNode()->GetTitle();
    }
    
    return FText::FromString("Pick a line");
}

FText SChronicle_DialogueLinkNode::GetSelectedNodeText() const
{
    if (TypedNode.IsValid() && TypedNode->GetLinkedNode())
    {
        return TypedNode->GetLinkedNode()->Text;
    }
    
    return FText::FromString("Node is not linked");
}

FText SChronicle_DialogueLinkNode::GetListenerName() const
{
    return FText::FromName(FChronicle_CharacterDirectory::GetAll().GetName(TypedNode->LinkedNode->ListenerId));
}

FText SChronicle_DialogueLinkNode::GetSpeakerName() const
{
    return FText::FromName(FChronicle_CharacterDirectory::GetAll().GetName(TypedNode->LinkedNode->SpeakerId));
}

void SChronicle_DialogueLinkNode::SelectNode(UChronicle_DialogueLineNode* Node) const
{
    if (!TypedNode.IsValid())
    {
        return;
    }
    
    if (!Node)
    {
        return;
    }

    TypedNode->Modify();
    TypedNode->SetLinkedNode(Node);

    if (UChronicle_DialogueGraph* Graph = Cast<UChronicle_DialogueGraph>(TypedNode->GetGraph()))
    {
        Graph->NotifyGraphChanged();
    }
}