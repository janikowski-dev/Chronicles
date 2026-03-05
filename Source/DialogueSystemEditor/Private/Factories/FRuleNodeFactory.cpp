#pragma once

#include "FRuleNodeFactory.h"

#include "FChronicleCharacterDirectory.h"
#include "FChronicle_RuleDirectory.h"
#include "Nodes/Slate/SRuleDefaultNode.h"
#include "Nodes/Slate/SRuleInputNode.h"
#include "Nodes/Unreal/URuleCallbackNode.h"
#include "Nodes/Unreal/URuleConditionNode.h"

TSharedPtr<SGraphNode> FRuleNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (URuleCallbackNode* TypedNode = Cast<URuleCallbackNode>(Node))
	{
		return SNew(SRuleInputNode, TypedNode, FChronicle_RuleDirectory::GetCallbacks(TypedNode->ParameterType), FChronicleCharacterDirectory::GetAll());
	}
	
	if (URuleConditionNode* TypedNode = Cast<URuleConditionNode>(Node))
	{
		return SNew(SRuleInputNode, TypedNode, FChronicle_RuleDirectory::GetConditions(TypedNode->ParameterType), FChronicleCharacterDirectory::GetAll());
	}
	
	if (URuleNode* TypedNode = Cast<URuleNode>(Node))
	{
		return SNew(SRuleDefaultNode, TypedNode);
	}
	
	return nullptr;
}
