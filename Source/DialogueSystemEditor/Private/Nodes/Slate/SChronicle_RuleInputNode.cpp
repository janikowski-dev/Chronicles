#include "SChronicle_RuleInputNode.h"

#include "FChronicle_CharacterSet.h"
#include "Nodes/Unreal/UChronicle_RuleInputNode.h"
#include "Widgets/Input/SNumericEntryBox.h"

void SChronicle_RuleInputNode::Construct(const FArguments&,
	UChronicle_RuleInputNode* InNode,
	const FChronicle_RuleSet& InRuleSet,
	const IChronicle_CharacterSet& InCharacterSet
)
{
	Cache(InNode);
	Cache(InRuleSet);
	Cache(InCharacterSet);
	UpdateGraphNode();
}

void SChronicle_RuleInputNode::UpdateGraphNode()
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
					GetInputBody()
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

TSharedRef<SWidget> SChronicle_RuleInputNode::GetInputBody() const
{
	switch (TypedNode->ParameterType)
	{
	case EChronicle_RuleParameterType::None:
		return GetRuleSelectionBody();
	case EChronicle_RuleParameterType::CharacterId:
		return GetCharacterSelectionBody();
	case EChronicle_RuleParameterType::Integer:
		return GetIntegerInputBody();
	}

	return GetIntegerInputBody();
}

TSharedRef<SWidget> SChronicle_RuleInputNode::GetIntegerInputBody() const
{
	return SNew(SVerticalBox)

	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		GetRuleSelectionBody()
	]
    
	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SNumericEntryBox<int32>)
		.Value_Lambda([this]() -> TOptional<int32>
		{
			return TypedNode->IntegerParameter;
		})
		.OnValueChanged_Lambda([this](const int32 NewValue)
		{
			TypedNode->IntegerParameter = NewValue;
		})
		.OnValueCommitted_Lambda([this](const int32 NewValue, ETextCommit::Type)
		{
			TypedNode->IntegerParameter = NewValue;
		})
		.MinValue(0)
		.MaxValue(20)
	];
}

TSharedRef<SWidget> SChronicle_RuleInputNode::GetCharacterSelectionBody() const
{
	return SNew(SVerticalBox)

	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		GetRuleSelectionBody()
	]
    
	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SComboBox<TSharedPtr<FGuid>>)
		.OptionsSource(&CharacterIds)
		.OnGenerateWidget_Lambda([this](const TSharedPtr<FGuid>& InItem)
		{
			return SNew(STextBlock)
			.Text(FText::FromName(CharacterSet->GetName(*InItem)));
		})
		.OnSelectionChanged_Lambda([this](const TSharedPtr<FGuid>& NewSelection, ESelectInfo::Type)
		{
			TypedNode->CharacterIdParameter = *NewSelection;
		})
		[
			SNew(STextBlock)
			.Text_Lambda([this]() -> FText
			{
				return CharacterSet->IsValid(TypedNode->CharacterIdParameter) ?
					FText::FromName(CharacterSet->GetName(TypedNode->CharacterIdParameter)) :
					FText::FromString("Select character...");
			})
		]
	];
}

TSharedRef<SWidget> SChronicle_RuleInputNode::GetRuleSelectionBody() const
{
	return SNew(SComboBox<TSharedPtr<FGuid>>)
	.OptionsSource(&RuleIds)
	.OnGenerateWidget_Lambda([this](const TSharedPtr<FGuid>& InItem)
	{
		return SNew(STextBlock)
		.Text(FText::FromName(RuleSet->GetName(*InItem)));
	})
	.OnSelectionChanged_Lambda([this](const TSharedPtr<FGuid>& NewSelection, ESelectInfo::Type)
	{
		TypedNode->RuleId = *NewSelection;
	})
	[
		SNew(STextBlock)
		.Text_Lambda([this]() -> FText
		{
			return RuleSet->IsValid(TypedNode->RuleId) ?
				FText::FromName(RuleSet->GetName(TypedNode->RuleId)) :
				FText::FromString("Select rule...");
		})
	];
}

void SChronicle_RuleInputNode::Cache(UChronicle_RuleInputNode* InNode)
{
	TypedNode = Cast<UChronicle_RuleInputNode>(InNode);
	GraphNode = InNode;
}

void SChronicle_RuleInputNode::Cache(const FChronicle_RuleSet& InSet)
{
	RuleIds = InSet.GetSharedIds();
	RuleSet = &InSet;
}

void SChronicle_RuleInputNode::Cache(const IChronicle_CharacterSet& InSet)
{
	CharacterIds = InSet.GetSharedIds();
	CharacterSet = &InSet;
}
