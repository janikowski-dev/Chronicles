#pragma once

#include "EdGraphUtilities.h"

class DIALOGUESYSTEMEDITOR_API FChronicle_RuleNodeFactory : public FGraphPanelNodeFactory
{
public:
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};
