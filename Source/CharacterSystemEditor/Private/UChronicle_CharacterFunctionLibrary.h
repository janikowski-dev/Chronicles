#pragma once

#include "FChronicle_CharacterModel.h"
#include "UChronicle_CharacterFunctionLibrary.generated.h"

UCLASS()
class CHARACTERSYSTEMEDITOR_API UChronicle_CharacterFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Chronicle|Characters")
	static FName GetName(const FGuid Id);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Characters")
	static TArray<FChronicle_CharacterModel> GetAllModels(const FGuid CharacterId);
	
	UFUNCTION(BlueprintCallable, Category="Chronicle|Characters")
	static FChronicle_CharacterModel GetModel(const FGuid ModelId);
};