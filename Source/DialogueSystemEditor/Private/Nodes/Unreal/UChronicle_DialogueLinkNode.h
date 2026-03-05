#pragma once

#include "CoreMinimal.h"
#include "UChronicle_DialogueNode.h"
#include "UChronicle_DialogueLinkNode.generated.h"

class UChronicle_DialogueLineNode;

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_DialogueLinkNode : public UChronicle_DialogueNode
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;
	virtual FText GetTitle() const override;

	UChronicle_DialogueLineNode* GetLinkedNode() const;
	void SetLinkedNode(UChronicle_DialogueLineNode* InNode);

public:
	UPROPERTY()
	TObjectPtr<UChronicle_DialogueLineNode> LinkedNode;
};