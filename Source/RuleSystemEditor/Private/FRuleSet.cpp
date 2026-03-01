#include "FRuleSet.h"

FRuleSet::FRuleSet(const TArray<FRule>& Rules, const ERuleParameterType& ParameterType)
	: ParameterType(ParameterType)
{
	for (const FRule& Rule : Rules)
	{
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

void FRuleSet::Refresh()
{
}
