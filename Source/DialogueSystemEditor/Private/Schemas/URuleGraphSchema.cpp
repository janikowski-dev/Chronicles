#include "URuleGraphSchema.h"

#include "ToolMenus.h"
#include "ToolMenuSection.h"
#include "Actions/FRuleGraph_AddNode.h"
#include "Actions/FRuleGraph_AddOutputNode.h"
#include "Nodes/Unreal/URuleAndNode.h"
#include "Nodes/Unreal/URuleCallbackNode.h"
#include "Nodes/Unreal/URuleConditionNode.h"
#include "Nodes/Unreal/URuleNotNode.h"
#include "Nodes/Unreal/URuleOrNode.h"
#include "Nodes/Unreal/URuleOutputNode.h"

void URuleGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	MakeShared<FRuleGraph_AddOutputNode>(
		URuleOutputNode::StaticClass(),
		EOutputType::Requirements
	)->PerformAction(&Graph, nullptr, FVector2f(0.0f, 0.0f));
	
	MakeShared<FRuleGraph_AddOutputNode>(
		URuleOutputNode::StaticClass(),
		EOutputType::PostCallback
	)->PerformAction(&Graph, nullptr, FVector2f(0.0f, 128.0f));
}

void URuleGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShared<FRuleGraph_AddNode>(
			FText::FromString("1. Conditions"),
			FText::FromString("1.1. Base Node"),
			FText::FromString("Adds a condition node without parameter"),
			URuleConditionNode::StaticClass(),
			ERuleParameterType::None
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FRuleGraph_AddNode>(
			FText::FromString("1. Conditions"),
			FText::FromString("1.2. Integer Node"),
			FText::FromString("Adds a condition node with integer parameter"),
			URuleConditionNode::StaticClass(),
			ERuleParameterType::Integer
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FRuleGraph_AddNode>(
			FText::FromString("1. Conditions"),
			FText::FromString("1.3. Character Node"),
			FText::FromString("Adds a condition node with character parameter"),
			URuleConditionNode::StaticClass(),
			ERuleParameterType::CharacterId
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FRuleGraph_AddNode>(
			FText::FromString("2. Operations"),
			FText::FromString("2.1. And Node"),
			FText::FromString("Adds an and node"),
			URuleAndNode::StaticClass(),
			ERuleParameterType::None
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FRuleGraph_AddNode>(
			FText::FromString("2. Operations"),
			FText::FromString("2.2. Or Node"),
			FText::FromString("Adds an or node"),
			URuleOrNode::StaticClass(),
			ERuleParameterType::None
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FRuleGraph_AddNode>(
			FText::FromString("2. Operations"),
			FText::FromString("2.3. Not Node"),
			FText::FromString("Adds a not node"),
			URuleNotNode::StaticClass(),
			ERuleParameterType::None
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FRuleGraph_AddNode>(
			FText::FromString("3. Callbacks"),
			FText::FromString("3.1. Base Node"),
			FText::FromString("Adds a callback node without parameter"),
			URuleCallbackNode::StaticClass(),
			ERuleParameterType::None
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FRuleGraph_AddNode>(
			FText::FromString("3. Callbacks"),
			FText::FromString("3.2. Integer Node"),
			FText::FromString("Adds a callback node with integer parameter"),
			URuleCallbackNode::StaticClass(),
			ERuleParameterType::Integer
		)
	);
	
	ContextMenuBuilder.AddAction(MakeShared<FRuleGraph_AddNode>(
			FText::FromString("3. Callbacks"),
			FText::FromString("3.3. Character Node"),
			FText::FromString("Adds a callback node with character parameter"),
			URuleCallbackNode::StaticClass(),
			ERuleParameterType::CharacterId
		)
	);
}

const FPinConnectionResponse URuleGraphSchema::CanCreateConnection(
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
	const URuleNode* InputNode = nullptr;

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
		InputNode = Cast<URuleNode>(InputPin->GetOwningNode());
	}

	if (InputNode && !InputNode->AllowMultipleInputs() && InputPin->LinkedTo.Num() > 0)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW,TEXT("This pin allows only one input"));
	}
	
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, "");
}
