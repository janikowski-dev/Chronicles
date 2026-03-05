#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "UChronicle_RuleGraphSchema.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_RuleGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	
	virtual void GetGraphContextActions(
		FGraphContextMenuBuilder& ContextMenuBuilder
	) const override;

	virtual const FPinConnectionResponse CanCreateConnection(
		const UEdGraphPin* A,
		const UEdGraphPin* B
	) const override;
};