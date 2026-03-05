#pragma once

#include "SChronicle_DialogueNode.h"

class UChronicle_RuleNode;

class SChronicle_RuleDefaultNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SChronicle_RuleDefaultNode) {}
	SLATE_END_ARGS()
	void Construct(const FArguments&, UChronicle_RuleNode* InNode);
	virtual void UpdateGraphNode() override;

private:
	void Cache(UChronicle_RuleNode* InNode);
	
private:
	TWeakObjectPtr<UChronicle_RuleNode> TypedNode;
};