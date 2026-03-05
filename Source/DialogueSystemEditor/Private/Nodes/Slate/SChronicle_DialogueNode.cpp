#include "SChronicle_DialogueNode.h"

#include "Nodes/Unreal/UChronicle_DialogueNode.h"
#include "Nodes/Unreal/UChronicle_DialogueLineNode.h"
#include "Nodes/Unreal/UChronicle_DialogueLinkNode.h"
#include "Nodes/Unreal/UChronicle_DialogueResponseNode.h"
#include "Nodes/Unreal/UChronicle_DialogueRootNode.h"

#include "Pins/SChronicle_DialogueGraphPin.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "SGraphPanel.h"
#include "Graphs/UChronicle_DialogueGraph.h"
#include "Utils/FChronicle_EditorStyle.h"

template<typename TNodeType>
void SChronicle_DialogueNode<TNodeType>::MoveTo(const FVector2f& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty)
{
}

template<typename TNodeType>
TSharedPtr<SGraphPin> SChronicle_DialogueNode<TNodeType>::CreatePinWidget(UEdGraphPin* Pin) const
{
	return SNew(SChronicle_DialogueGraphPin, Pin);
}

template<typename TNodeType>
TSharedPtr<SGraphPin> SChronicle_DialogueNode<TNodeType>::GetHoveredPin(const FGeometry&, const FPointerEvent&) const
{
	return nullptr;
}

template <typename TNodeType>
const FSlateBrush* SChronicle_DialogueNode<TNodeType>::GetShadowBrush(bool bSelected) const
{
	if (!bSelected)
	{
		return SGraphNode::GetShadowBrush(bSelected);
	}
	
	return FChronicle_EditorStyle::Get().GetBrush("Node.Highlight");
}

template<typename TNodeType>
void SChronicle_DialogueNode<TNodeType>::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();
	SetupErrorReporting();

	const TSharedRef<SVerticalBox> RegularView = SNew(SVerticalBox);
	const TSharedRef<SVerticalBox> CollapsedView = SNew(SVerticalBox);
	const TSharedRef<SWidget> HiddenView = SNew(SSpacer);
	
	SAssignNew(LeftNodeBox, SVerticalBox);
	SAssignNew(RightNodeBox, SVerticalBox);

	AddHeader(CollapsedView);
	AddHeader(RegularView);
	AddBody(RegularView);
	
	SAssignNew(WidgetSwitcher, SWidgetSwitcher)
	.WidgetIndex(this, &SChronicle_DialogueNode::GetBodyIndex)
	+ SWidgetSwitcher::Slot()
	[
		RegularView
	]

	+ SWidgetSwitcher::Slot()
	[
		CollapsedView
	]

	+ SWidgetSwitcher::Slot()
	[
		HiddenView
	];

	RemoveSlot(ENodeZone::Center);
	
	GetOrAddSlot(ENodeZone::Center)
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
		.BorderBackgroundColor(this, &SChronicle_DialogueNode::GetHeaderColor)
		[
			WidgetSwitcher.ToSharedRef()
		]
	];
	
	CreatePinWidgets();
}

template<typename TNodeType>
void SChronicle_DialogueNode<TNodeType>::AddHeader(const TSharedRef<SVerticalBox>& Box)
{
	const FText Title = TypedNode->GetTitle();
	
	Box->AddSlot()
	.AutoHeight()
	[
		SNew(SOverlay)
		
		+ SOverlay::Slot()
		[
			SNew(SBox)
			.WidthOverride(225.0f)
			.Padding(6)
			[
				SNew(STextBlock)
				.Text(Title)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			]
		]

		+ SOverlay::Slot()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 4.0f))
			.AutoWidth()
			[
				LeftNodeBox.ToSharedRef()
			]
			
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SSpacer)
			]

			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 4.0f))
			.AutoWidth()
			[
				RightNodeBox.ToSharedRef()
			]
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		[
			SNew(SBox)
			.WidthOverride(30.0f)
			[
				SNew(SButton)
				.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
				.ContentPadding(FMargin(4))
				.ClickMethod(EButtonClickMethod::MouseDown)
				.OnClicked(this, &SChronicle_DialogueNode::ToggleCollapse)
				[
					SNew(SImage)
					.Image_Lambda([this]
					{
						return TypedNode->bIsCollapsed
							? FAppStyle::Get().GetBrush("Icons.ChevronRight")
							: FAppStyle::Get().GetBrush("Icons.ChevronDown");
					})
				]
			]
		]
	];
}

template<typename TNodeType>
FReply SChronicle_DialogueNode<TNodeType>::ToggleCollapse() const
{
	ToggleCollapsedState();
	ApplyCollapse();
	RefreshGraph();
	return FReply::Handled();
}

template<typename TNodeType>
void SChronicle_DialogueNode<TNodeType>::ApplyCollapse() const
{
	TSet<UChronicle_DialogueNode*> Visited;
	ApplyCollapse(TypedNode.Get(), false, Visited);
}

template<typename TNodeType>
void SChronicle_DialogueNode<TNodeType>::ApplyCollapse(
	UChronicle_DialogueNode* CurrentNode,
	const bool bParentHidden,
	TSet<UChronicle_DialogueNode*>& Visited
) const
{
	if (!CurrentNode || Visited.Contains(CurrentNode))
	{
		return;
	}

	Visited.Add(CurrentNode);
	CurrentNode->bIsHidden = bParentHidden;
	const bool bHideChildren = bParentHidden || CurrentNode->bIsCollapsed;

	for (UEdGraphPin* Pin : CurrentNode->Pins)
	{
		if (Pin->Direction != EGPD_Output)
		{
			continue;
		}

		for (const UEdGraphPin* LinkedPin : Pin->LinkedTo)
		{
			if (UChronicle_DialogueNode* ChildNode = Cast<UChronicle_DialogueNode>(LinkedPin->GetOwningNode()))
			{
				ApplyCollapse(ChildNode, bHideChildren, Visited);
			}
		}
	}
}

template<typename TNodeType>
void SChronicle_DialogueNode<TNodeType>::ToggleCollapsedState() const
{
	TypedNode->bIsCollapsed = !TypedNode->bIsCollapsed;
}

template<typename TNodeType>
void SChronicle_DialogueNode<TNodeType>::RefreshGraph() const
{
	TypedGraph->Refresh();
}

template<typename TNodeType>
int SChronicle_DialogueNode<TNodeType>::GetBodyIndex() const
{
	if (TypedNode->bIsCollapsed)
	{
		return Collapsed;
	}

	if (TypedNode->bIsHidden)
	{
		return Hidden;
	}
	
	return Regular;
}

template<typename TNodeType>
void SChronicle_DialogueNode<TNodeType>::Cache(UEdGraphNode* Node)
{
	TypedGraph = Cast<UChronicle_DialogueGraph>(Node->GetGraph());
	TypedNode = Cast<TNodeType>(Node);
	GraphNode = Node;
}

template class SChronicle_DialogueNode<UChronicle_DialogueResponseNode>;
template class SChronicle_DialogueNode<UChronicle_DialogueLineNode>;
template class SChronicle_DialogueNode<UChronicle_DialogueLinkNode>;
template class SChronicle_DialogueNode<UChronicle_DialogueRootNode>;
