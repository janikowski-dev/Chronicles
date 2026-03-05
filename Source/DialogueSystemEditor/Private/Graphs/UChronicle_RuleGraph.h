#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "UChronicle_RuleGraph.generated.h"

enum class EOutputType : int8;
class UChronicle_RuleNode;

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_RuleGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	TArray<UChronicle_RuleNode*> GetRules(EOutputType Type) const;

private:
	UChronicle_RuleNode* GetOutputNode(const EOutputType Type) const;
	void GetRules(UChronicle_RuleNode* Node, TArray<UChronicle_RuleNode*>& Rules) const;
};
