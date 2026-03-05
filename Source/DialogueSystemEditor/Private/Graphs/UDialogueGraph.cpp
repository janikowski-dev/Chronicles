#include "UDialogueGraph.h"

#include "FChronicleCharacterDirectory.h"
#include "Nodes/Unreal/UDialogueLineNode.h"
#include "Nodes/Unreal/UDialogueResponseNode.h"
#include "Nodes/Unreal/UDialogueRootNode.h"

struct FDialogueLayoutNode
{
	UDialogueNode* Node = nullptr;
	TArray<FDialogueLayoutNode*> Children;
	float Y = 0.0f;
};

void UDialogueGraph::PostLoad()
{
	Super::PostLoad();
	Refresh();
}

void UDialogueGraph::Refresh()
{
	ApplyLayout();
	IndexLines();
	IndexResponses();
	CacheLines();
}

void UDialogueGraph::Serialize(FStructuredArchive::FRecord Record)
{
	TrySave(Record);
	
	Super::Serialize(Record);
    
	if (TryLoad(Record))
	{
		SortParticipants();
	}
}

bool UDialogueGraph::HasParticipant(const TSharedPtr<FGuid>& Id) const
{
	return HasParticipantInternal(Id);
}

void UDialogueGraph::AddParticipant(const TSharedPtr<FGuid>& Id)
{
	Modify();
	AddParticipantInternal(Id);
	SortParticipants();

	if (MarkPackageDirty())
	{
		NotifyGraphChanged();
	}
}

void UDialogueGraph::RemoveParticipant(const TSharedPtr<FGuid>& Id)
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

void UDialogueGraph::ApplyLayout() const
{
	constexpr float CellWidth  = 350.0f;
	constexpr float CellHeight = 300.0f;
	float CursorY = 0.0f;

	TMap<UDialogueNode*, FDialogueLayoutNode*> LayoutMap;
	FDialogueLayoutNode* Layout = BuildLayoutTree(GetRootNode(), LayoutMap);
	LayoutSubtree(Layout, CursorY, CellHeight);
	ApplyLayout(Layout, 0, CellWidth);
	
	for (const TPair<UDialogueNode*, FDialogueLayoutNode*>& Iterator : LayoutMap)
	{
		delete Iterator.Value;
	}
}

void UDialogueGraph::IndexLines() const
{
	int32 NodeIndex = 0;
	TSet<UDialogueNode*> Visited;
	IndexLines(GetRootNode(), NodeIndex, Visited);
}

UDialogueNode* UDialogueGraph::GetRootNode() const
{
	for (UEdGraphNode* Node : Nodes)
	{
		if (UDialogueRootNode* Root = Cast<UDialogueRootNode>(Node))
		{
			return Root;
		}
	}
	
	return nullptr;
}

FDialogueLayoutNode* UDialogueGraph::BuildLayoutTree(
	UDialogueNode* Node,
	TMap<UDialogueNode*, FDialogueLayoutNode*>& LayoutMap
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
			UDialogueNode* Child = Cast<UDialogueNode>(Linked->GetOwningNode());
			
			if (!Child)
			{
				continue;
			}
			
			Layout->Children.Add(BuildLayoutTree(Child, LayoutMap));
		}
	}

	return Layout;
}

float UDialogueGraph::LayoutSubtree(
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

void UDialogueGraph::ApplyLayout(
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

void UDialogueGraph::IndexLines(
	UDialogueNode* Node,
	int32& NodeIndex,
	TSet<UDialogueNode*>& Visited
) const
{
	if (!Node || Visited.Contains(Node))
	{
		return;
	}

	Visited.Add(Node);

	if (UDialogueLineNode* Line = Cast<UDialogueLineNode>(Node))
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
			if (UDialogueNode* Child = Cast<UDialogueNode>(Linked->GetOwningNode()))
			{
				IndexLines(Child, NodeIndex, Visited);
			}
		}
	}
}

void UDialogueGraph::IndexResponses() const
{
	for (UEdGraphNode* GraphNode : Nodes)
	{
		UDialogueLineNode* Line = Cast<UDialogueLineNode>(GraphNode);
		
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
				if (UDialogueResponseNode* Response = Cast<UDialogueResponseNode>(Linked->GetOwningNode()))
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

bool UDialogueGraph::TryLoad(FStructuredArchive::FRecord Record)
{
	if (!Record.GetUnderlyingArchive().IsLoading())
	{
		return false;
	}

	SharedParticipantIds.Reset();
	
	for (const TSharedPtr<FGuid>& Id : FChronicleCharacterDirectory::GetAll().GetSharedIds())
	{
		if (ParticipantIds.Contains(*Id))
		{
			SharedParticipantIds.Add(Id);
		}
	}

	return true;
}

bool UDialogueGraph::TrySave(FStructuredArchive::FRecord Record)
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

bool UDialogueGraph::HasParticipantInternal(const TSharedPtr<FGuid>& Id) const
{
	return SharedParticipantIds.Contains(Id);
}

void UDialogueGraph::AddParticipantInternal(const TSharedPtr<FGuid>& Id)
{
	SharedParticipantIds.Add(Id);
	ParticipantIds.Add(*Id);
}

void UDialogueGraph::RemoveParticipantInternal(const TSharedPtr<FGuid>& Id)
{
	SharedParticipantIds.Remove(Id);
	ParticipantIds.Remove(*Id);
}

void UDialogueGraph::SortParticipants()
{
	SharedParticipantIds.Sort([](const TSharedPtr<FGuid>& A, const TSharedPtr<FGuid>& B)
	{
		const bool bAIsPlayer = FChronicleCharacterDirectory::GetPlayable().IsValid(*A);
		const bool bBIsPlayer = FChronicleCharacterDirectory::GetPlayable().IsValid(*B);

		if (bAIsPlayer != bBIsPlayer)
		{
			return bAIsPlayer;
		}

		return false;
	});
}


void UDialogueGraph::CacheLines()
{
	LineNodes.Empty();

	for (UEdGraphNode* Node : Nodes)
	{
		if (UDialogueLineNode* LineNode = Cast<UDialogueLineNode>(Node))
		{
			LineNodes.Add(LineNode);
		}
	}
    
	LineNodes.Sort([](const TWeakObjectPtr<UDialogueLineNode>& A, const TWeakObjectPtr<UDialogueLineNode>& B)
	{
		return A->GetTitle().ToString() < B->GetTitle().ToString();
	});
}

#pragma endregion
