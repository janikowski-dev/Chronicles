#include "UChronicle_CharacterFunctionLibrary.h"

#include "UChronicle_CharacterSettings.h"

FName UChronicle_CharacterFunctionLibrary::GetName(const FGuid Id)
{
	return GetDefault<UChronicle_CharacterSettings>()->GetName(Id);
}

TArray<FChronicle_CharacterModel> UChronicle_CharacterFunctionLibrary::GetAllModels(const FGuid CharacterId)
{
	return GetDefault<UChronicle_CharacterSettings>()->GetAllModels(CharacterId);
}

FChronicle_CharacterModel UChronicle_CharacterFunctionLibrary::GetModel(const FGuid ModelId)
{
	return GetDefault<UChronicle_CharacterSettings>()->GetModel(ModelId);
}
