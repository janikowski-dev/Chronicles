#include "FRuleSet.h"

#include "FRule.h"

FRuleSet::FRuleSet(const ERuleParameterType& ParameterType) : ParameterType(ParameterType)
{
}

void FRuleSet::Refresh(const TArray<FRule>& Rules)
{
	for (const FRule& Rule : Rules)
	{
		const bool bContainsId = Ids.ContainsByPredicate([&](const TSharedPtr<FGuid>& Id)
		{
			return *Id == Rule.Id;
		});
		
		if (bContainsId)
		{
			continue;
		}
		
		TSharedPtr<FGuid> SharedId = MakeShared<FGuid>(Rule.Id);
		NamesById.Add(Rule.Id, Rule.Name);
		Ids.Add(SharedId);
	}
}

FName FRuleSet::GetName(const FGuid Id) const
{
	if (FName const* Found = NamesById.Find(Id))
	{
		return *Found;
	}

	return NAME_None;
}

TArray<TSharedPtr<FGuid>> FRuleSet::GetSharedIds() const
{
	return Ids;
}

bool FRuleSet::IsValid(const FGuid Id) const
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
