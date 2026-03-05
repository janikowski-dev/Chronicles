#pragma once

#include "FChronicle_RuleNodeFactory.h"

#include "FChronicle_CharacterDirectory.h"
#include "FChronicle_RuleDirectory.h"
#include "Nodes/Slate/SChronicle_RuleDefaultNode.h"
#include "Nodes/Slate/SChronicle_RuleInputNode.h"
#include "Nodes/Unreal/UChronicle_RuleCallbackNode.h"
#include "Nodes/Unreal/UChronicle_RuleConditionNode.h"

TSharedPtr<SGraphNode> FChronicle_RuleNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UChronicle_RuleCallbackNode* TypedNode = Cast<UChronicle_RuleCallbackNode>(Node))
	{
		return SNew(SChronicle_RuleInputNode, TypedNode, FChronicle_RuleDirectory::GetCallbacks(TypedNode->ParameterType), FChronicle_CharacterDirectory::GetAll());
	}
	
	if (UChronicle_RuleConditionNode* TypedNode = Cast<UChronicle_RuleConditionNode>(Node))
	{
		return SNew(SChronicle_RuleInputNode, TypedNode, FChronicle_RuleDirectory::GetConditions(TypedNode->ParameterType), FChronicle_CharacterDirectory::GetAll());
	}
	
	if (UChronicle_RuleNode* TypedNode = Cast<UChronicle_RuleNode>(Node))
	{
		return SNew(SChronicle_RuleDefaultNode, TypedNode);
	}
	
	return nullptr;
}
