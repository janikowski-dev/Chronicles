#pragma once

#include "FChronicle_RuleSet.h"
#include "SChronicle_DialogueNode.h"
#include "IChronicle_CharacterSet.h"

class UChronicle_RuleInputNode;

class SChronicle_RuleInputNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SChronicle_RuleInputNode) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments&,
		UChronicle_RuleInputNode* InNode,
		const FChronicle_RuleSet& InRuleSet,
		const IChronicle_CharacterSet& InCharacterSet
	);
	
	virtual void UpdateGraphNode() override;

private:
	TSharedRef<SWidget> GetInputBody() const;
	TSharedRef<SWidget> GetRuleSelectionBody() const;
	TSharedRef<SWidget> GetIntegerInputBody() const;
	TSharedRef<SWidget> GetCharacterSelectionBody() const;
	
	void Cache(UChronicle_RuleInputNode* InNode);
	void Cache(const FChronicle_RuleSet& InSet);
	void Cache(const IChronicle_CharacterSet& InSet);
	
private:
	TWeakObjectPtr<UChronicle_RuleInputNode> TypedNode;
	
	const IChronicle_CharacterSet* CharacterSet = nullptr;
	const FChronicle_RuleSet* RuleSet = nullptr;
	
	TArray<TSharedPtr<FGuid>> CharacterIds;
	TArray<TSharedPtr<FGuid>> RuleIds;
};