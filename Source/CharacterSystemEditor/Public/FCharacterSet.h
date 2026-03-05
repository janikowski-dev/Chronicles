#pragma once

struct FChronicleCharacter;
class UCharacterAsset;

class CHARACTERSYSTEMEDITOR_API FCharacterSet
{
public:
	void Refresh(const TArray<FChronicleCharacter>& Characters);
	TArray<TSharedPtr<FGuid>> GetSharedIds() const;
	FName GetName(FGuid Id) const;
	bool IsValid(FGuid Id) const;
	
private:
	TArray<TSharedPtr<FGuid>> Ids;
	TMap<FGuid, FName> NamesById;
	FName Directory;
};
