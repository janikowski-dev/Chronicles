#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "UChronicle_DialogueAssetFactory.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_DialogueAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UChronicle_DialogueAssetFactory();

	virtual UObject* FactoryCreateNew(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		FFeedbackContext* Warn
	) override;
};