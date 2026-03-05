#include "UChronicle_DialogueGraph.h"

#include "FChronicle_CharacterDirectory.h"
#include "Nodes/Unreal/UChronicle_DialogueLineNode.h"
#include "Nodes/Unreal/UChronicle_DialogueResponseNode.h"
#include "Nodes/Unreal/UChronicle_DialogueRootNode.h"

struct FDialogueLayoutNode
{
	UChronicle_DialogueNode* Node = nullptr;
	TArray<FDialogueLayoutNode*> Children;
	float Y = 0.0f;
};

void UChronicle_DialogueGraph::PostLoad()
{
	Super::PostLoad();
	Refresh();
}

void UChronicle_DialogueGraph::Refresh()
{
	ApplyLayout();
	IndexLines();
	IndexResponses();
	CacheLines();
}

void UChronicle_DialogueGraph::Serialize(FStructuredArchive::FRecord Record)
{
	TrySave(Record);
	
	Super::Serialize(Record);
    
	if (TryLoad(Record))
	{
		SortParticipants();
	}
}

bool UChronicle_DialogueGraph::HasParticipant(const TSharedPtr<FGuid>& Id) const
{
	return HasParticipantInternal(Id);
}

void UChronicle_DialogueGraph::AddParticipant(const TSharedPtr<FGuid>& Id)
{
	Modify();
	AddParticipantInternal(Id);
	SortParticipants();

	if (MarkPackageDirty())
	{
		NotifyGraphChanged();
	}
}

void UChronicle_DialogueGraph::RemoveParticipant(const TSharedPtr<FGuid>& Id)
{
	Modify();
	RemoveParticipantInternal(Id);
	SortParticipants();

	if (MarkPackageDirty())
	{
		NotifyGraphChanged();
	}
}

#pragma region Layout

void UChronicle_DialogueGraph::ApplyLayout() const
{
	constexpr float CellWidth  = 350.0f;
	constexpr float CellHeight = 300.0f;
	float CursorY = 0.0f;

	TMap<UChronicle_DialogueNode*, FDialogueLayoutNode*> LayoutMap;
	FDialogueLayoutNode* Layout = BuildLayoutTree(GetRootNode(), LayoutMap);
	LayoutSubtree(Layout, CursorY, CellHeight);
	ApplyLayout(Layout, 0, CellWidth);
	
	for (const TPair<UChronicle_DialogueNode*, FDialogueLayoutNode*>& Iterator : LayoutMap)
	{
		delete Iterator.Value;
	}
}

void UChronicle_DialogueGraph::IndexLines() const
{
	int32 NodeIndex = 0;
	TSet<UChronicle_DialogueNode*> Visited;
	IndexLines(GetRootNode(), NodeIndex, Visited);
}

UChronicle_DialogueNode* UChronicle_DialogueGraph::GetRootNode() const
{
	for (UEdGraphNode* Node : Nodes)
	{
		if (UChronicle_DialogueRootNode* Root = Cast<UChronicle_DialogueRootNode>(Node))
		{
			return Root;
		}
	}
	
	return nullptr;
}

FDialogueLayoutNode* UChronicle_DialogueGraph::BuildLayoutTree(
	UChronicle_DialogueNode* Node,
	TMap<UChronicle_DialogueNode*, FDialogueLayoutNode*>& LayoutMap
) const
{
	FDialogueLayoutNode*& Layout = LayoutMap.FindOrAdd(Node);
	
	if (Layout)
	{
		return Layout;
	}
	
	Layout = new FDialogueLayoutNode{ Node };

	for (UEdGraphPin* Pin : Node->Pins)
	{
		if (Pin->Direction != EGPD_Output)
		{
			continue;
		}

		for (const UEdGraphPin* Linked : Pin->LinkedTo)
		{
			UChronicle_DialogueNode* Child = Cast<UChronicle_DialogueNode>(Linked->GetOwningNode());
			
			if (!Child)
			{
				continue;
			}
			
			Layout->Children.Add(BuildLayoutTree(Child, LayoutMap));
		}
	}

	return Layout;
}

float UChronicle_DialogueGraph::LayoutSubtree(
	FDialogueLayoutNode* Node,
	float& CursorY,
	const float VerticalSpacing
) const
{
	if (Node->Children.IsEmpty())
	{
		Node->Y = CursorY;
		CursorY += VerticalSpacing;
		return Node->Y;
	}

	float MinY = TNumericLimits<float>::Max();
	float MaxY = TNumericLimits<float>::Lowest();

	for (FDialogueLayoutNode* Child : Node->Children)
	{
		const float ChildY = LayoutSubtree(Child, CursorY, VerticalSpacing);
		MinY = FMath::Min(MinY, ChildY);
		MaxY = FMath::Max(MaxY, ChildY);
	}

	Node->Y = (MinY + MaxY) * 0.5f;
	return Node->Y;
}

void UChronicle_DialogueGraph::ApplyLayout(
	FDialogueLayoutNode* LayoutNode,
	const int32 Depth,
	const float CellWidth
) const
{
	constexpr float InvisibleAndOutOfSightNodeX = -100000.0f;
	
	if (!LayoutNode || !LayoutNode->Node)
	{
		return;
	}

	if (LayoutNode->Node->bIsHidden)
	{
		LayoutNode->Node->NodePosX = InvisibleAndOutOfSightNodeX;
	}
	else
	{
		LayoutNode->Node->NodePosX = Depth * CellWidth;
	}
	
	LayoutNode->Node->NodePosY = LayoutNode->Y;

	for (FDialogueLayoutNode* Child : LayoutNode->Children)
	{
		ApplyLayout(Child, Depth + 1, CellWidth);
	}
}

void UChronicle_DialogueGraph::IndexLines(
	UChronicle_DialogueNode* Node,
	int32& NodeIndex,
	TSet<UChronicle_DialogueNode*>& Visited
) const
{
	if (!Node || Visited.Contains(Node))
	{
		return;
	}

	Visited.Add(Node);

	if (UChronicle_DialogueLineNode* Line = Cast<UChronicle_DialogueLineNode>(Node))
	{
		Line->LineIndex = ++NodeIndex;
	}

	for (UEdGraphPin* Pin : Node->Pins)
	{
		if (Pin->Direction != EGPD_Output)
		{
			continue;
		}

		for (const UEdGraphPin* Linked : Pin->LinkedTo)
		{
			if (UChronicle_DialogueNode* Child = Cast<UChronicle_DialogueNode>(Linked->GetOwningNode()))
			{
				IndexLines(Child, NodeIndex, Visited);
			}
		}
	}
}

void UChronicle_DialogueGraph::IndexResponses() const
{
	for (UEdGraphNode* GraphNode : Nodes)
	{
		UChronicle_DialogueLineNode* Line = Cast<UChronicle_DialogueLineNode>(GraphNode);
		
		if (!Line)
		{
			continue;
		}

		int32 ChildIndex = 0;

		for (UEdGraphPin* Pin : Line->Pins)
		{
			if (Pin->Direction != EGPD_Output)
			{
				continue;
			}

			for (const UEdGraphPin* Linked : Pin->LinkedTo)
			{
				if (UChronicle_DialogueResponseNode* Response = Cast<UChronicle_DialogueResponseNode>(Linked->GetOwningNode()))
				{
					Response->ParentIndex = Line->LineIndex;
					Response->OrderIndex = ++ChildIndex;
				}
			}
		}
	}
}

#pragma endregion

#pragma region Participants

bool UChronicle_DialogueGraph::TryLoad(FStructuredArchive::FRecord Record)
{
	if (!Record.GetUnderlyingArchive().IsLoading())
	{
		return false;
	}

	SharedParticipantIds.Reset();
	
	for (const TSharedPtr<FGuid>& Id : FChronicle_CharacterDirectory::GetAll().GetSharedIds())
	{
		if (ParticipantIds.Contains(*Id))
		{
			SharedParticipantIds.Add(Id);
		}
	}

	return true;
}

bool UChronicle_DialogueGraph::TrySave(FStructuredArchive::FRecord Record)
{
	if (!Record.GetUnderlyingArchive().IsSaving())
	{
		return false;
	}

	ParticipantIds.Reset();
    
	for (const TSharedPtr<FGuid>& Id : SharedParticipantIds)
	{
		if (Id)
		{
			ParticipantIds.Add(*Id);
		}
	}

	return true;
}

bool UChronicle_DialogueGraph::HasParticipantInternal(const TSharedPtr<FGuid>& Id) const
{
	return SharedParticipantIds.Contains(Id);
}

void UChronicle_DialogueGraph::AddParticipantInternal(const TSharedPtr<FGuid>& Id)
{
	SharedParticipantIds.Add(Id);
	ParticipantIds.Add(*Id);
}

void UChronicle_DialogueGraph::RemoveParticipantInternal(const TSharedPtr<FGuid>& Id)
{
	SharedParticipantIds.Remove(Id);
	ParticipantIds.Remove(*Id);
}

void UChronicle_DialogueGraph::SortParticipants()
{
	SharedParticipantIds.Sort([](const TSharedPtr<FGuid>& A, const TSharedPtr<FGuid>& B)
	{
		const bool bAIsPlayer = FChronicle_CharacterDirectory::GetPlayable().IsValid(*A);
		const bool bBIsPlayer = FChronicle_CharacterDirectory::GetPlayable().IsValid(*B);

		if (bAIsPlayer != bBIsPlayer)
		{
			return bAIsPlayer;
		}

		return false;
	});
}


void UChronicle_DialogueGraph::CacheLines()
{
	LineNodes.Empty();

	for (UEdGraphNode* Node : Nodes)
	{
		if (UChronicle_DialogueLineNode* LineNode = Cast<UChronicle_DialogueLineNode>(Node))
		{
			LineNodes.Add(LineNode);
		}
	}
    
	LineNodes.Sort([](const TWeakObjectPtr<UChronicle_DialogueLineNode>& A, const TWeakObjectPtr<UChronicle_DialogueLineNode>& B)
	{
		return A->GetTitle().ToString() < B->GetTitle().ToString();
	});
}

#pragma endregion
