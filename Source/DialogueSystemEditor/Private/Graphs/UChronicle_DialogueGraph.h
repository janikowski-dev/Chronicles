#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "UChronicle_DialogueGraph.generated.h"

struct FDialogueLayoutNode;
class UChronicle_DialogueLineNode;
class UChronicle_DialogueNode;

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_DialogueGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	virtual void PostLoad() override;
	virtual void Serialize(FStructuredArchive::FRecord Record) override;
	
	UChronicle_DialogueNode* GetRootNode() const;
	void Refresh();

	bool HasParticipant(const TSharedPtr<FGuid>& Id) const;
	void AddParticipant(const TSharedPtr<FGuid>& Id);
	void RemoveParticipant(const TSharedPtr<FGuid>& Id);
	
private:
	void ApplyLayout() const;
	void IndexLines() const;
	
	FDialogueLayoutNode* BuildLayoutTree(
		UChronicle_DialogueNode* Node,
		TMap<UChronicle_DialogueNode*, FDialogueLayoutNode*>& LayoutMap
	) const;
	
	float LayoutSubtree(
		FDialogueLayoutNode* Node,
		float& CursorY,
		float VerticalSpacing
	) const;
	
	void ApplyLayout(
		FDialogueLayoutNode* LayoutNode,
		int32 Depth,
		float CellWidth
	) const;

	void IndexLines(
		UChronicle_DialogueNode* Node,
		int32& NodeIndex,
		TSet<UChronicle_DialogueNode*>& Visited
	) const;

	void IndexResponses() const;

	bool TrySave(FStructuredArchive::FRecord Record);
	bool TryLoad(FStructuredArchive::FRecord Record);
	
	bool HasParticipantInternal(const TSharedPtr<FGuid>& Id) const;
	void AddParticipantInternal(const TSharedPtr<FGuid>& Id);
	void RemoveParticipantInternal(const TSharedPtr<FGuid>& Id);
	
	void SortParticipants();

	void CacheLines();

public:
	TArray<TWeakObjectPtr<UChronicle_DialogueLineNode>> LineNodes;
	TArray<TSharedPtr<FGuid>> SharedParticipantIds;
	
	UPROPERTY()
	TArray<FGuid> ParticipantIds;
};