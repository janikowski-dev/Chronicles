#pragma once

#include "FRule.h"

enum class ERuleParameterType : uint8;
class URuleAsset;

class RULESYSTEMEDITOR_API FRuleSet
{
public:
	FRuleSet() = default;
	explicit FRuleSet(const TArray<FRule>& Rules, const ERuleParameterType& ParameterType);
	
public:
	TArray<TSharedPtr<FGuid>> GetSharedIds() const;
	FName GetName(FGuid Id) const;
	bool IsValid(FGuid Id) const;
	
	void Refresh();
	
private:
	ERuleParameterType ParameterType;
	TArray<TSharedPtr<FGuid>> Ids;
	TMap<FGuid, FName> NamesById;
};
