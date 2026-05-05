#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AChronicle_CharacterActor.generated.h"

UCLASS()
class CHARACTERSYSTEM_API AChronicle_CharacterActor : public ACharacter
{
	GENERATED_BODY()

public:
	void PlayAnimation(TObjectPtr<UAnimSequence> Sequence);
};
