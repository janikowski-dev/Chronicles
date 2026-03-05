#pragma once

#include "ERuleParameterType.h"

struct FRuleGraph_AddNode : FEdGraphSchemaAction
{
	TSubclassOf<UEdGraphNode> NodeClass;
	ERuleParameterType ParameterType;

	explicit FRuleGraph_AddNode(const TSubclassOf<UEdGraphNode> InNodeClass)
		: NodeClass(InNodeClass)
		, ParameterType(ERuleParameterType::None)
	{
	}
	
	explicit FRuleGraph_AddNode(
		const FText& InCategory,
		const FText& InMenuDesc,
		const FText& InTooltip,
		const TSubclassOf<UEdGraphNode> InNodeClass,
		const ERuleParameterType InParameterType
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
