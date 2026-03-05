#pragma once

enum class ERuleParameterType : uint8;
class URuleAsset;
struct FRule;

class RULESYSTEMEDITOR_API FRuleSet
{
public:
	FRuleSet() = default;
	explicit FRuleSet(const ERuleParameterType& ParameterType);
	
public:
	void Refresh(const TArray<FRule>& Rules);
	TArray<TSharedPtr<FGuid>> GetSharedIds() const;
	FName GetName(FGuid Id) const;
	bool IsValid(FGuid Id) const;
	
private:
	ERuleParameterType ParameterType;
	TArray<TSharedPtr<FGuid>> Ids;
	TMap<FGuid, FName> NamesById;
};
