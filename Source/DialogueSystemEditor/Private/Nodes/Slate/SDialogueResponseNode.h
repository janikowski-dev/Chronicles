#pragma once

#include "SDialogueNode.h"

class UDialogueResponseNode;

class SDialogueResponseNode : public SDialogueNode<UDialogueResponseNode>
{
public:
	SLATE_BEGIN_ARGS(SDialogueResponseNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UDialogueResponseNode* InNode);

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
