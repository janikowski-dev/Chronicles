#include "UChronicle_RuleGraphSchema.h"

#include "ToolMenus.h"
#include "ToolMenuSection.h"
#include "Actions/FChronicle_RuleGraph_AddNode.h"
#include "Actions/FChronicle_RuleGraph_AddOutputNode.h"
#include "Nodes/Unreal/UChronicle_RuleAndNode.h"
#include "Nodes/Unreal/UChronicle_RuleCallbackNode.h"
#include "Nodes/Unreal/UChronicle_RuleConditionNode.h"
#include "Nodes/Unreal/UChronicle_RuleNotNode.h"
#include "Nodes/Unreal/UChronicle_RuleOrNode.h"
#include "Nodes/Unreal/UChronicle_RuleOutputNode.h"

void UChronicle_RuleGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	MakeShared<FChronicle_RuleGraph_AddOutputNode>(
		UChronicle_RuleOutputNode::StaticClass(),
		EOutputType::Requirements
	)->PerformAction(&Graph, nullptr, FVector2f(0.0f, 0.0f));
	
	MakeShared<FChronicle_RuleGraph_AddOutputNode>(
		UChronicle_RuleOutputNode::StaticClass(),
		EOutputType::PostCallback
	)->PerformAction(&Graph, nullptr, FVector2f(0.0f, 128.0f));
}

void UChronicle_RuleGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShared<FChronicle_RuleGraph_AddNode>(
			FText::FromString("1. Conditions"),
			FText::FromString("1.1. Base Node"),
			FText::FromString("Adds a condition node without parameter"),
			UChronicle_RuleConditionNode::StaticClass(),
			EChronicle_RuleParameterType::None
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FChronicle_RuleGraph_AddNode>(
			FText::FromString("1. Conditions"),
			FText::FromString("1.2. Integer Node"),
			FText::FromString("Adds a condition node with integer parameter"),
			UChronicle_RuleConditionNode::StaticClass(),
			EChronicle_RuleParameterType::Integer
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FChronicle_RuleGraph_AddNode>(
			FText::FromString("1. Conditions"),
			FText::FromString("1.3. Character Node"),
			FText::FromString("Adds a condition node with character parameter"),
			UChronicle_RuleConditionNode::StaticClass(),
			EChronicle_RuleParameterType::CharacterId
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FChronicle_RuleGraph_AddNode>(
			FText::FromString("2. Operations"),
			FText::FromString("2.1. And Node"),
			FText::FromString("Adds an and node"),
			UChronicle_RuleAndNode::StaticClass(),
			EChronicle_RuleParameterType::None
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FChronicle_RuleGraph_AddNode>(
			FText::FromString("2. Operations"),
			FText::FromString("2.2. Or Node"),
			FText::FromString("Adds an or node"),
			UChronicle_RuleOrNode::StaticClass(),
			EChronicle_RuleParameterType::None
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FChronicle_RuleGraph_AddNode>(
			FText::FromString("2. Operations"),
			FText::FromString("2.3. Not Node"),
			FText::FromString("Adds a not node"),
			UChronicle_RuleNotNode::StaticClass(),
			EChronicle_RuleParameterType::None
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FChronicle_RuleGraph_AddNode>(
			FText::FromString("3. Callbacks"),
			FText::FromString("3.1. Base Node"),
			FText::FromString("Adds a callback node without parameter"),
			UChronicle_RuleCallbackNode::StaticClass(),
			EChronicle_RuleParameterType::None
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FChronicle_RuleGraph_AddNode>(
			FText::FromString("3. Callbacks"),
			FText::FromString("3.2. Integer Node"),
			FText::FromString("Adds a callback node with integer parameter"),
			UChronicle_RuleCallbackNode::StaticClass(),
			EChronicle_RuleParameterType::Integer
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FChronicle_RuleGraph_AddNode>(
			FText::FromString("3. Callbacks"),
			FText::FromString("3.3. Character Node"),
			FText::FromString("Adds a callback node with character parameter"),
			UChronicle_RuleCallbackNode::StaticClass(),
			EChronicle_RuleParameterType::CharacterId
		)
	);
}

const FPinConnectionResponse UChronicle_RuleGraphSchema::CanCreateConnection(
	const UEdGraphPin* A,
	const UEdGraphPin* B
) const
{
	if (A->PinType.PinCategory != B->PinType.PinCategory)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Nodes don't belong to the same category"));
	}
	
	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect same direction pins"));
	}
	
	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins from the same node"));
	}

	if (A->LinkedTo.Contains(B))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins are already connected"));
	}

	const UEdGraphPin* InputPin = nullptr;
	const UChronicle_RuleNode* InputNode = nullptr;

	if (A->Direction == EGPD_Input)
	{
		InputPin = A;
	}
	else if (B->Direction == EGPD_Input)
	{
		InputPin = B;
	}

	if (InputPin)
	{
		InputNode = Cast<UChronicle_RuleNode>(InputPin->GetOwningNode());
	}

	if (InputNode && !InputNode->AllowMultipleInputs() && InputPin->LinkedTo.Num() > 0)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW,TEXT("This pin allows only one input"));
	}
	
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, "");
}
