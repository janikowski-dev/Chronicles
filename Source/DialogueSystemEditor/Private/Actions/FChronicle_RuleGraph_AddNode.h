#pragma once

#include "EChronicle_RuleParameterType.h"

struct FChronicle_RuleGraph_AddNode : FEdGraphSchemaAction
{
	TSubclassOf<UEdGraphNode> NodeClass;
	EChronicle_RuleParameterType ParameterType;

	explicit FChronicle_RuleGraph_AddNode(const TSubclassOf<UEdGraphNode> InNodeClass)
		: NodeClass(InNodeClass)
		, ParameterType(EChronicle_RuleParameterType::None)
	{
	}
	
	explicit FChronicle_RuleGraph_AddNode(
		const FText& InCategory,
		const FText& InMenuDesc,
		const FText& InTooltip,
		const TSubclassOf<UEdGraphNode> InNodeClass,
		const EChronicle_RuleParameterType InParameterType
	)
		: FEdGraphSchemaAction(InCategory, InMenuDesc, InTooltip, 0)
		, NodeClass(InNodeClass)
		, ParameterType(InParameterType)
	{
	}

	virtual UEdGraphNode* PerformAction(
		UEdGraph* ParentGraph,
		UEdGraphPin* FromPin,
		const FVector2f& Location,
		bool bSelectNewNode = true
	) override;
	
	UEdGraphNode* CreateNewNode(UEdGraph* ParentGraph, const FVector2f& Location) const;
};
