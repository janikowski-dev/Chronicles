#pragma once

#include "EdGraphUtilities.h"

class DIALOGUESYSTEMEDITOR_API FChronicle_DialogueNodeFactory : public FGraphPanelNodeFactory
{
public:
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};
