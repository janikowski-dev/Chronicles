#include "UChronicle_DialogueGraphSchema.h"

#include "Policies/FChronicle_DialogueNodeConnectionDrawingPolicy.h"
#include "Actions/FChronicle_DialogueGraph_AddNode.h"
#include "Nodes/Unreal/UChronicle_DialogueResponseNode.h"
#include "Nodes/Unreal/UChronicle_DialogueRootNode.h"
#include "Nodes/Unreal/UChronicle_DialogueLineNode.h"
#include "ToolMenus.h"
#include "ToolMenuSection.h"
#include "Actions/FChronicle_DialogueGraph_DeleteNodeWithChildren.h"
#include "Actions/FChronicle_DialogueGraph_FocusRoot.h"
#include "Nodes/Unreal/UChronicle_DialogueLinkNode.h"

#pragma region Initialization

void UChronicle_DialogueGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	MakeShared<FChronicle_DialogueGraph_AddNode>(
		UChronicle_DialogueRootNode::StaticClass()
	)->PerformAction(&Graph,nullptr, FVector2f::Zero());
}

#pragma endregion

#pragma region Connection

FConnectionDrawingPolicy* UChronicle_DialogueGraphSchema::CreateConnectionDrawingPolicy(
	const int32 InBackLayerID,
	const int32 InFrontLayerID,
	const float InZoomFactor,
	const FSlateRect& InClippingRect,
	FSlateWindowElementList& InDrawElements,
	UEdGraph* InGraphObj
) const
{
	return new FChronicle_DialogueNodeConnectionDrawingPolicy(
		InBackLayerID,
		InFrontLayerID,
		InZoomFactor,
		InClippingRect,
		InDrawElements
	);
}

const FPinConnectionResponse UChronicle_DialogueGraphSchema::CanCreateConnection(
	const UEdGraphPin* A,
	const UEdGraphPin* B
) const
{
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

bool UChronicle_DialogueGraphSchema::TryCreateConnection(
	UEdGraphPin* A,
	UEdGraphPin* B
) const
{
	A->Modify();
	B->Modify();
	A->MakeLinkTo(B);
	return true;
}

#pragma endregion

#pragma region Additional Actions

void UChronicle_DialogueGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShared<FChronicle_DialogueGraph_FocusRoot>());
}

#pragma endregion

#pragma region Node Creation

void UChronicle_DialogueGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection* Section = CreateSection(Menu, Context);

	if (!Section)
	{
		return;
	}

	AddLineContext(Context, Section);
	AddResponseContext(Context, Section);
	AddRootContext(Context, Section);
	AddLinkContext(Context, Section);
}

void UChronicle_DialogueGraphSchema::AddLineContext(const UGraphNodeContextMenuContext* Context, FToolMenuSection* Section) const
{
	const UChronicle_DialogueLineNode* Node = Cast<UChronicle_DialogueLineNode>(Context->Node);
	
	if (!Node)
	{
		return;
	}
	
	AddResponseAction(Context, Section);
	AddLineAction(Context, Section);
	AddLinkAction(Context, Section);
	AddDeleteAction(Context, Section);
}

void UChronicle_DialogueGraphSchema::AddLinkContext(const UGraphNodeContextMenuContext* Context, FToolMenuSection* Section) const
{
	const UChronicle_DialogueLinkNode* Node = Cast<UChronicle_DialogueLinkNode>(Context->Node);
	
	if (!Node)
	{
		return;
	}
	
	AddDeleteAction(Context, Section);
}

void UChronicle_DialogueGraphSchema::AddResponseContext(const UGraphNodeContextMenuContext* Context, FToolMenuSection* Section) const
{
	const UChronicle_DialogueResponseNode* Node = Cast<UChronicle_DialogueResponseNode>(Context->Node);
	
	if (!Node)
	{
		return;
	}
	
	if (!HasAnyChild(Node))
	{
		AddLineAction(Context, Section);
		AddLinkAction(Context, Section);
	}
	
	AddDeleteAction(Context, Section);
}

void UChronicle_DialogueGraphSchema::AddRootContext(const UGraphNodeContextMenuContext* Context, FToolMenuSection* Section) const
{
	const UChronicle_DialogueRootNode* Node = Cast<UChronicle_DialogueRootNode>(Context->Node);
	
	if (!Node)
	{
		return;
	}
	
	if (HasAnyChild(Node))
	{
		return;
	}
	
	AddLineAction(Context, Section);
}

void UChronicle_DialogueGraphSchema::AddLineAction(const UGraphNodeContextMenuContext* Context, FToolMenuSection* Section) const
{
	UEdGraph* Graph = const_cast<UEdGraph*>(Context->Graph.Get());
	UEdGraphPin* Pin = GetOutputPin(Context->Node);
	
	Section->AddMenuEntry(
		"AddLineNode",
		FText::FromString("Line Node"),
		FText::FromString("Adds a line node"),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([Graph, Pin]
			{
				MakeShared<FChronicle_DialogueGraph_AddNode>(
					UChronicle_DialogueLineNode::StaticClass()
				)->PerformAction(Graph, Pin, FVector2f::Zero(), true);
			})
		)
	);
}

void UChronicle_DialogueGraphSchema::AddLinkAction(const UGraphNodeContextMenuContext* Context, FToolMenuSection* Section) const
{
	UEdGraph* Graph = const_cast<UEdGraph*>(Context->Graph.Get());
	UEdGraphPin* Pin = GetOutputPin(Context->Node);
	
	Section->AddMenuEntry(
		"AddLinkNode",
		FText::FromString("Link Node"),
		FText::FromString("Adds a link node"),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([Graph, Pin]
			{
				MakeShared<FChronicle_DialogueGraph_AddNode>(
					UChronicle_DialogueLinkNode::StaticClass()
				)->PerformAction(Graph, Pin, FVector2f::Zero(), true);
			})
		)
	);
}

void UChronicle_DialogueGraphSchema::AddResponseAction(const UGraphNodeContextMenuContext* Context, FToolMenuSection* Section) const
{
	UEdGraph* Graph = const_cast<UEdGraph*>(Context->Graph.Get());
	UEdGraphPin* Pin = GetOutputPin(Context->Node);
	
	Section->AddMenuEntry(
		"AddResponseNode",
		FText::FromString("Response Node"),
		FText::FromString("Adds a response node"),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([Graph, Pin]
			{
				MakeShared<FChronicle_DialogueGraph_AddNode>(
					UChronicle_DialogueResponseNode::StaticClass()
				)->PerformAction(Graph, Pin, FVector2f::Zero(), true);
			})
		)
	);
}

void UChronicle_DialogueGraphSchema::AddDeleteAction(const UGraphNodeContextMenuContext* Context, FToolMenuSection* Section) const
{
	UEdGraph* Graph = const_cast<UEdGraph*>(Context->Graph.Get());
	UEdGraphPin* Pin = Context->Node->Pins[0];
	
	Section->AddMenuEntry(
		"DeleteNode",
		FText::FromString("Delete Node"),
		FText::FromString("Deletes node and its' children"),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([Graph, Pin]
			{
				MakeShared<FChronicle_DialogueGraph_DeleteNodeWithChildren>()->PerformAction(Graph, Pin, FVector2f::Zero(), true);
			})
		)
	);
}

bool UChronicle_DialogueGraphSchema::HasChildOfType(const UEdGraphNode* Node, UClass* ChildClass) const
{
	for (const UEdGraphPin* Pin : Node->Pins)
	{
		if (Pin->Direction != EGPD_Output)
		{
			continue;
		}

		for (const UEdGraphPin* Linked : Pin->LinkedTo)
		{
			if (!Linked || !Linked->GetOwningNode()->IsA(ChildClass))
			{
				continue;
			}
			
			return true;
		}
	}

	return false;
}

bool UChronicle_DialogueGraphSchema::HasAnyChild(const UEdGraphNode* Node) const
{
	for (const UEdGraphPin* Pin : Node->Pins)
	{
		if (Pin->Direction != EGPD_Output)
		{
			continue;
		}
		
		for (const UEdGraphPin* Linked : Pin->LinkedTo)
		{
			if (Linked)
			{
				return true;
			}
		}
	}

	return false;
}

FToolMenuSection* UChronicle_DialogueGraphSchema::CreateSection(UToolMenu* Menu, const UGraphNodeContextMenuContext* Context) const
{
	if (!Context->Node)
	{
		return nullptr;
	}
	
	return &Menu->AddSection("SchemaNode", FText::FromString("Schema"));
}

UEdGraphPin* UChronicle_DialogueGraphSchema::GetOutputPin(const UEdGraphNode* Node) const
{
	for (UEdGraphPin* Pin : Node->Pins)
	{
		if (Pin && Pin->Direction == EGPD_Output)
		{
			return Pin;
		}
	}
	return nullptr;
}

#pragma endregion
