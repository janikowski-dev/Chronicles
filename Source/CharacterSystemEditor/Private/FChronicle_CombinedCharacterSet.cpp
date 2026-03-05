#include "FChronicle_CombinedCharacterSet.h"

FChronicle_CombinedCharacterSet::FChronicle_CombinedCharacterSet(FChronicle_CharacterSet& InSet1, FChronicle_CharacterSet& InSet2)
	: Set1(InSet1)
	, Set2(InSet2)
{
}

void FChronicle_CombinedCharacterSet::Refresh(const TArray<FChronicle_Character>& Characters)
{
}

TArray<TSharedPtr<FGuid>> FChronicle_CombinedCharacterSet::GetSharedIds() const
{
	TArray<TSharedPtr<FGuid>> Combined = Set1.GetSharedIds();
	Combined.Append(Set2.GetSharedIds());
	return Combined;
}

FName FChronicle_CombinedCharacterSet::GetName(const FGuid Id) const
{
	if (const FName Name = Set1.GetName(Id); !Name.IsNone())
	{
		return Name;
	}

	if (const FName Name = Set2.GetName(Id); !Name.IsNone())
	{
		return Name;
	}

	return NAME_None;
}

bool FChronicle_CombinedCharacterSet::IsValid(const FGuid Id) const
{
	return Set1.IsValid(Id) || Set2.IsValid(Id);
}
