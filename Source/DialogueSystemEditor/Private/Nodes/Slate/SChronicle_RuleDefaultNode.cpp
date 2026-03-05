#include "SChronicle_RuleDefaultNode.h"

#include "Nodes/Unreal/UChronicle_RuleInputNode.h"

void SChronicle_RuleDefaultNode::Construct(const FArguments&, UChronicle_RuleNode* InNode)
{
	Cache(InNode);
	UpdateGraphNode();
}

void SChronicle_RuleDefaultNode::UpdateGraphNode()
{
	SetupErrorReporting();

	RemoveSlot(ENodeZone::Center);

	GetOrAddSlot(ENodeZone::Center)
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
		.BorderBackgroundColor(TypedNode->GetBackgroundColor())
		[
			SNew(SVerticalBox)
			
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				.Padding(4)
				[
					SNew(STextBlock)
					.Text(TypedNode->GetTitle())
				]
			]
			
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
			
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SAssignNew(LeftNodeBox, SVerticalBox)
				]
				
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				[
					SNew(SSpacer)
				]
			
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SAssignNew(RightNodeBox, SVerticalBox)
				]
			]
		]
	];
	
	CreatePinWidgets();
}

void SChronicle_RuleDefaultNode::Cache(UChronicle_RuleNode* InNode)
{
	TypedNode = Cast<UChronicle_RuleNode>(InNode);
	GraphNode = InNode;
}
