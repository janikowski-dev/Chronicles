#include "SRuleInputNode.h"

#include "FCharacterSet.h"
#include "Nodes/Unreal/URuleInputNode.h"
#include "Widgets/Input/SNumericEntryBox.h"

void SRuleInputNode::Construct(const FArguments&,
	URuleInputNode* InNode,
	const FRuleSet& InRuleSet,
	const FCharacterSet& InCharacterSet
)
{
	Cache(InNode);
	Cache(InRuleSet);
	Cache(InCharacterSet);
	UpdateGraphNode();
}

void SRuleInputNode::UpdateGraphNode()
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

TSharedRef<SWidget> SRuleInputNode::GetInputBody() const
{
	switch (TypedNode->ParameterType)
	{
	case ERuleParameterType::None:
		return GetRuleSelectionBody();
	case ERuleParameterType::CharacterId:
		return GetCharacterSelectionBody();
	case ERuleParameterType::Integer:
		return GetIntegerInputBody();
	}

	return GetIntegerInputBody();
}

TSharedRef<SWidget> SRuleInputNode::GetIntegerInputBody() const
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

TSharedRef<SWidget> SRuleInputNode::GetCharacterSelectionBody() const
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

TSharedRef<SWidget> SRuleInputNode::GetRuleSelectionBody() const
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

void SRuleInputNode::Cache(URuleInputNode* InNode)
{
	TypedNode = Cast<URuleInputNode>(InNode);
	GraphNode = InNode;
}

void SRuleInputNode::Cache(const FRuleSet& InSet)
{
	RuleSet = const_cast<FRuleSet*>(&InSet);
	RuleIds = InSet.GetSharedIds();
}

void SRuleInputNode::Cache(const FCharacterSet& InSet)
{
	CharacterSet = const_cast<FCharacterSet*>(&InSet);
	CharacterIds = InSet.GetSharedIds();
}
