#pragma once

#include "CoreMinimal.h"
#include "UDialogueNode.h"
#include "UDialogueResponseNode.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UDialogueResponseNode : public UDialogueNode
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