#pragma once

#include "CoreMinimal.h"
#include "UChronicle_DialogueNode.h"
#include "UChronicle_DialogueResponseNode.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_DialogueResponseNode : public UChronicle_DialogueNode
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;
	virtual FText GetTitle() const override;
	virtual FText GetText() const override;
	
public:
	UPROPERTY()
	int32 ParentIndex;
	UPROPERTY()
	int32 OrderIndex;
	UPROPERTY()
	FText Text;
	UPROPERTY()
	FGuid ListenerId;
};