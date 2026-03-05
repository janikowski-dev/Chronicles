#pragma once

#include "CoreMinimal.h"
#include "UChronicle_DialogueNode.h"
#include "UChronicle_DialogueLineNode.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_DialogueLineNode : public UChronicle_DialogueNode
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;
	virtual FText GetTitle() const override;
	virtual FText GetText() const override;
	
public:
	UPROPERTY(EditAnywhere)
	FText Text;
	UPROPERTY()
	int32 LineIndex;
	UPROPERTY()
	FGuid SpeakerId;
	UPROPERTY()
	FGuid ListenerId;
};