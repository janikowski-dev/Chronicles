#pragma once

struct FChronicle_DialogueGraph_FocusRoot : FEdGraphSchemaAction
{
	FChronicle_DialogueGraph_FocusRoot()
		: FEdGraphSchemaAction(
			FText::FromString("Navigation"),
			FText::FromString("Focus on Root"),
			FText::FromString("Center the graph on the Root node"),
			0
		)
	{}

	virtual UEdGraphNode* PerformAction(
		UEdGraph* ParentGraph,
		UEdGraphPin* FromPin,
		const FVector2f& Location,
		bool bSelectNewNode
	) override;
};