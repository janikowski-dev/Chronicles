#pragma once

#include "IChronicle_CharacterSet.h"

struct FChronicle_Character;
class UCharacterAsset;

class CHARACTERSYSTEMEDITOR_API FChronicle_CharacterSet : public IChronicle_CharacterSet
{
public:
	virtual void Refresh(const TArray<FChronicle_Character>& Characters) override;
	virtual TArray<TSharedPtr<FGuid>> GetSharedIds() const override;
	virtual FName GetName(FGuid Id) const override;
	virtual bool IsValid(FGuid Id) const override;
	
private:
	TArray<TSharedPtr<FGuid>> Ids;
	TMap<FGuid, FName> NamesById;
	FName Directory;
};
