#pragma once

#include "FRuleNodeFactory.h"

#include "FCharacterDirectory.h"
#include "URuleSettings.h"
#include "Nodes/Slate/SRuleDefaultNode.h"
#include "Nodes/Slate/SRuleInputNode.h"
#include "Nodes/Unreal/URuleCallbackNode.h"
#include "Nodes/Unreal/URuleConditionNode.h"

TSharedPtr<SGraphNode> FRuleNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (URuleCallbackNode* TypedNode = Cast<URuleCallbackNode>(Node))
	{
		return SNew(SRuleInputNode, TypedNode, GetDefault<URuleSettings>()->GetCallbacks(TypedNode->ParameterType), FCharacterDirectory::GetAll());
	}
	
	if (URuleConditionNode* TypedNode = Cast<URuleConditionNode>(Node))
	{
		return SNew(SRuleInputNode, TypedNode, GetDefault<URuleSettings>()->GetConditions(TypedNode->ParameterType), FCharacterDirectory::GetAll());
	}
	
	if (URuleNode* TypedNode = Cast<URuleNode>(Node))
	{
		return SNew(SRuleDefaultNode, TypedNode);
	}
	
	return nullptr;
}
