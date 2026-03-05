#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "UChronicle_DialogueGraphSchema.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_DialogueGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;

	virtual FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(
		int32 InBackLayerID,
		int32 InFrontLayerID,
		float InZoomFactor,
		const FSlateRect& InClippingRect,
		FSlateWindowElementList& InDrawElements,
		UEdGraph* InGraphObj)
	const override;

	virtual void GetGraphContextActions(
		FGraphContextMenuBuilder& ContextMenuBuilder
	) const override;

	virtual void GetContextMenuActions(
		UToolMenu* Menu,
		UGraphNodeContextMenuContext* Context
	) const override;

	virtual const FPinConnectionResponse CanCreateConnection(
		const UEdGraphPin* A,
		const UEdGraphPin* B
	) const override;

	virtual bool TryCreateConnection(
		UEdGraphPin* A,
		UEdGraphPin* B
	) const override;
	
private:
	void AddLineAction(
		const UGraphNodeContextMenuContext* Context,
		FToolMenuSection* Section
	) const;
	
	void AddLinkAction(
		const UGraphNodeContextMenuContext* Context,
		FToolMenuSection* Section
	) const;

	void AddResponseAction(
		const UGraphNodeContextMenuContext* Context,
		FToolMenuSection* Section
	) const;

	void AddDeleteAction(
		const UGraphNodeContextMenuContext* Context,
		FToolMenuSection* Section
	) const;

	bool HasChildOfType(
		const UEdGraphNode* Node,
		UClass* ChildClass
	) const;

	bool HasAnyChild(
		const UEdGraphNode* Node
	) const;

	void AddLineContext(
		const UGraphNodeContextMenuContext* Context,
		FToolMenuSection* Section
	) const;

	void AddLinkContext(
		const UGraphNodeContextMenuContext* Context,
		FToolMenuSection* Section
	) const;

	void AddResponseContext(
		const UGraphNodeContextMenuContext* Context,
		FToolMenuSection* Section
	) const;

	void AddRootContext(
		const UGraphNodeContextMenuContext* Context,
		FToolMenuSection* Section
	) const;

	FToolMenuSection* CreateSection(
		UToolMenu* Menu,
		const UGraphNodeContextMenuContext* Context
	) const;

	UEdGraphPin* GetOutputPin(const UEdGraphNode* Node) const;
};