#pragma once

#include "SGraphNode.h"
#include "Graphs/UChronicle_DialogueGraph.h"

class UChronicle_DialogueNode;
class UChronicle_DialogueResponseNode;
class UChronicle_DialogueLineNode;
class UChronicle_DialogueLinkNode;
class UChronicle_DialogueRootNode;

enum ENodeViewType : int32
{
	Regular = 0,
	Collapsed = 1,
	Hidden = 2
};

template<typename TNodeType>
class SChronicle_DialogueNode : public SGraphNode
{
public:
	virtual void MoveTo(const FVector2f& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty = true) override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;
	virtual TSharedPtr<SGraphPin> GetHoveredPin(const FGeometry&, const FPointerEvent&) const override;
	virtual const FSlateBrush* GetShadowBrush(bool bSelected) const override;
	virtual void UpdateGraphNode() override;

protected:
	virtual void AddBody(const TSharedRef<SVerticalBox>& Box) = 0;
	virtual FSlateColor GetHeaderColor() const = 0;
	
	void Cache(UEdGraphNode* Node);

private:
	void AddHeader(const TSharedRef<SVerticalBox>& Box);
	void ApplyCollapse() const;
	void RefreshGraph() const;
	void ApplyCollapse(
		UChronicle_DialogueNode* CurrentNode,
		bool bParentHidden,
		TSet<UChronicle_DialogueNode*>& Visited
	) const;
	void ToggleCollapsedState() const;
	FReply ToggleCollapse() const;
	int GetBodyIndex() const;
	
protected:
	TWeakObjectPtr<UChronicle_DialogueGraph> TypedGraph;
	TWeakObjectPtr<TNodeType> TypedNode;

private:
	TSharedPtr<SWidgetSwitcher> WidgetSwitcher;
};

extern template class SChronicle_DialogueNode<UChronicle_DialogueResponseNode>;
extern template class SChronicle_DialogueNode<UChronicle_DialogueLineNode>;
extern template class SChronicle_DialogueNode<UChronicle_DialogueLinkNode>;
extern template class SChronicle_DialogueNode<UChronicle_DialogueRootNode>;
