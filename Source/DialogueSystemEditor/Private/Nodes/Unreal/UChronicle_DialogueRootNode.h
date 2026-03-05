#pragma once

#include "CoreMinimal.h"
#include "UChronicle_DialogueNode.h"
#include "UChronicle_DialogueRootNode.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_DialogueRootNode : public UChronicle_DialogueNode
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;
	virtual FText GetTitle() const override;
};