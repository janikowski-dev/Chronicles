#include "FCharacterSet.h"

#include "FChronicleCharacter.h"

void FCharacterSet::Refresh(const TArray<FChronicleCharacter>& Characters)
{
	for (const FChronicleCharacter& Character : Characters)
	{
		const bool bContainsId = Ids.ContainsByPredicate([&](const TSharedPtr<FGuid>& Id)
		{
			return *Id == Character.Id;
		});
		
		if (bContainsId)
		{
			continue;
		}
		
		TSharedPtr<FGuid> SharedId = MakeShared<FGuid>(Character.Id);
		NamesById.Add(Character.Id, Character.Name);
		Ids.Add(SharedId);
	}
}

FName FCharacterSet::GetName(const FGuid Id) const
{
	if (FName const* Found = NamesById.Find(Id))
	{
		return *Found;
	}

	return NAME_None;
}

TArray<TSharedPtr<FGuid>> FCharacterSet::GetSharedIds() const
{
	return Ids;
}

bool FCharacterSet::IsValid(const FGuid Id) const
{
	for (const TSharedPtr<FGuid>& IdPointer : Ids)
	{
		if (IdPointer.IsValid() && *IdPointer == Id)
		{
			return true;
		}
	}
	return false;
}
