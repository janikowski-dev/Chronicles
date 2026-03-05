#pragma once

#include "SChronicle_DialogueNode.h"

class UChronicle_DialogueResponseNode;

class SChronicle_DialogueResponseNode : public SChronicle_DialogueNode<UChronicle_DialogueResponseNode>
{
public:
	SLATE_BEGIN_ARGS(SChronicle_DialogueResponseNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UChronicle_DialogueResponseNode* InNode);

protected:
	virtual void AddBody(const TSharedRef<SVerticalBox>& Box) override;
	virtual FSlateColor GetHeaderColor() const override;
	virtual void UpdateGraphNode() override;
	virtual FReply OnMouseButtonDoubleClick(const FGeometry&, const FPointerEvent&) override;

private:
	FText GetText() const;
	void SetText(const FText& NewText, ETextCommit::Type) const;
	void OpenNodeEditor() const;
	void FixAssignedId() const;
};
