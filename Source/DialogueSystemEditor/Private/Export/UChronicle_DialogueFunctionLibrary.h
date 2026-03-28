#pragma once

#include "Assets/UChronicle_DialogueAsset.h"
#include "Data/UChronicle_DialogueData.h"
#include "UChronicle_DialogueFunctionLibrary.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_DialogueFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Chronicle|Dialogue")
    static UChronicle_DialogueData* ConvertToData(const UChronicle_DialogueAsset* Asset);
    
    UFUNCTION(BlueprintCallable, Category="Chronicle|Dialogue")
    static FString ConvertToJson(const UChronicle_DialogueAsset* Asset);
};