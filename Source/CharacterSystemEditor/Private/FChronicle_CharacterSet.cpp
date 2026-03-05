#include "FChronicle_CharacterSet.h"

#include "FChronicle_Character.h"

void FChronicle_CharacterSet::Refresh(const TArray<FChronicle_Character>& Characters)
{
	for (const FChronicle_Character& Character : Characters)
	{
		const bool bContainsId = Ids.ContainsByPredicate([&](const TSharedPtr<FGuid>& Id)
		{
			return *Id == Character.Id;
		});
		
		if (bContainsId)
		{
			NamesById[Character.Id] = Character.Name;
		}
		else
		{
			TSharedPtr<FGuid> SharedId = MakeShared<FGuid>(Character.Id);
			NamesById.Add(Character.Id, Character.Name);
			Ids.Add(SharedId);
		}
	}
}

FName FChronicle_CharacterSet::GetName(const FGuid Id) const
{
	if (FName const* Found = NamesById.Find(Id))
	{
		return *Found;
	}

	return NAME_None;
}

TArray<TSharedPtr<FGuid>> FChronicle_CharacterSet::GetSharedIds() const
{
	return Ids;
}

bool FChronicle_CharacterSet::IsValid(const FGuid Id) const
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
