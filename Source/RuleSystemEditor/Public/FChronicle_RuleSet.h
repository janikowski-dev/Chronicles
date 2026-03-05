#pragma once

enum class EChronicle_RuleParameterType : uint8;
class URuleAsset;
struct FChronicle_Rule;

class RULESYSTEMEDITOR_API FChronicle_RuleSet
{
public:
	FChronicle_RuleSet() = default;
	explicit FChronicle_RuleSet(const EChronicle_RuleParameterType& ParameterType);
	
public:
	void Refresh(const TArray<FChronicle_Rule>& Rules);
	TArray<TSharedPtr<FGuid>> GetSharedIds() const;
	FName GetName(FGuid Id) const;
	bool IsValid(FGuid Id) const;
	
private:
	EChronicle_RuleParameterType ParameterType;
	TArray<TSharedPtr<FGuid>> Ids;
	TMap<FGuid, FName> NamesById;
};
