#include "UChronicle_CharacterSettings.h"

FName UChronicle_CharacterSettings::GetCategoryName() const
{
	return TEXT("Chronicle");
}

FName UChronicle_CharacterSettings::GetName(const FGuid Id) const
{
	for (const FChronicle_Character& Character : NonPlayableCharacters)
	{
		if (Character.Id == Id)
		{
			return Character.Name;
		}
	}
	
	for (const FChronicle_Character& Character : PlayableCharacters)
	{
		if (Character.Id == Id)
		{
			return Character.Name;
		}
	}

	return TEXT("");
}

TArray<FChronicle_CharacterModel> UChronicle_CharacterSettings::GetAllModels(const FGuid CharacterId) const
{
	for (const FChronicle_Character& Character : NonPlayableCharacters)
	{
		if (Character.Id == CharacterId)
		{
			return Character.Models;
		}
	}
	
	for (const FChronicle_Character& Character : PlayableCharacters)
	{
		if (Character.Id == CharacterId)
		{
			return Character.Models;
		}
	}

	return {};
}

FChronicle_CharacterModel UChronicle_CharacterSettings::GetModel(FGuid ModelId) const
{
	for (const FChronicle_Character& Character : NonPlayableCharacters)
	{
		for (FChronicle_CharacterModel Model : Character.Models)
		{
			if (Model.Id == ModelId)
			{
				return Model;
			}
		}
	}
	
	for (const FChronicle_Character& Character : PlayableCharacters)
	{
		for (FChronicle_CharacterModel Model : Character.Models)
		{
			if (Model.Id == ModelId)
			{
				return Model;
			}
		}
	}

	return {};
}
