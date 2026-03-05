#include "FChronicle_RuleSet.h"

#include "FChronicle_Rule.h"

FChronicle_RuleSet::FChronicle_RuleSet(const EChronicle_RuleParameterType& ParameterType) : ParameterType(ParameterType)
{
}

void FChronicle_RuleSet::Refresh(const TArray<FChronicle_Rule>& Rules)
{
	for (const FChronicle_Rule& Rule : Rules)
	{
		const bool bContainsId = Ids.ContainsByPredicate([&](const TSharedPtr<FGuid>& Id)
		{
			return *Id == Rule.Id;
		});
		
		if (bContainsId)
		{
			NamesById[Rule.Id] = Rule.Name;
		}
		else
		{
			TSharedPtr<FGuid> SharedId = MakeShared<FGuid>(Rule.Id);
			NamesById.Add(Rule.Id, Rule.Name);
			Ids.Add(SharedId);
		}
	}
}

FName FChronicle_RuleSet::GetName(const FGuid Id) const
{
	if (FName const* Found = NamesById.Find(Id))
	{
		return *Found;
	}

	return NAME_None;
}

TArray<TSharedPtr<FGuid>> FChronicle_RuleSet::GetSharedIds() const
{
	return Ids;
}

bool FChronicle_RuleSet::IsValid(const FGuid Id) const
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
