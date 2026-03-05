#pragma once

#include "SChronicle_DialogueNode.h"

class UChronicle_DialogueLinkNode;
class UChronicle_DialogueLineNode;

class SChronicle_DialogueLinkNode : public SChronicle_DialogueNode<UChronicle_DialogueLinkNode>
{
public:
	SLATE_BEGIN_ARGS(SChronicle_DialogueLinkNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UChronicle_DialogueLinkNode* InNode);

protected:
	virtual void AddBody(const TSharedRef<SVerticalBox>& Box) override;
	virtual FSlateColor GetHeaderColor() const override;
	
private:
	TSharedRef<SWidget> GetLineNodesMenu();
	void SelectNode(UChronicle_DialogueLineNode* Node) const;
	FText GetSelectedNodeTitle() const;
	FText GetSelectedNodeText() const;
	FText GetListenerName() const;
	FText GetSpeakerName() const;
};