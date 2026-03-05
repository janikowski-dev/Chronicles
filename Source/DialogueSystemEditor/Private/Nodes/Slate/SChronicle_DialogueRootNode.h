#pragma once

#include "SChronicle_DialogueNode.h"

class UChronicle_DialogueRootNode;

class SChronicle_DialogueRootNode : public SChronicle_DialogueNode<UChronicle_DialogueRootNode>
{
public:
	SLATE_BEGIN_ARGS(SChronicle_DialogueRootNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments&, UChronicle_DialogueRootNode* InNode);

protected:
	virtual void AddBody(const TSharedRef<SVerticalBox>& Box) override;
	virtual FSlateColor GetHeaderColor() const override;
	FReply OpenAddParticipantWindow() const;

private:
	void AddCurrentParticipantList(const TSharedRef<SVerticalBox>& Box) const;
	void AddNewParticipantButton(const TSharedRef<SVerticalBox>& Box) const;
};
