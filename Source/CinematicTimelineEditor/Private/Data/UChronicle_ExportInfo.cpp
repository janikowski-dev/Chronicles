#include "UChronicle_ExportInfo.h"
#include "UChronicle_CinematicData.h"

FName UChronicle_ExportInfo::GetCategoryName() const
{
	return TEXT("Chronicle");
}

FChronicle_DialogueInfo UChronicle_ExportInfo::Get(const FGuid& DialogueInfoId)
{
	FChronicle_DialogueInfo* MatchingInfo = Infos.FindByPredicate([&DialogueInfoId](const FChronicle_DialogueInfo& Entry)
	{
		return DialogueInfoId == Entry.Id;
	});

	if (!MatchingInfo)
	{
		return {};
	}

	return *MatchingInfo;
}

void UChronicle_ExportInfo::Override(const FGuid& DialogueInfoId, const FChronicle_SequenceInfo& SequenceInfo)
{
	FChronicle_DialogueInfo* MatchingInfo = Infos.FindByPredicate([&DialogueInfoId](const FChronicle_DialogueInfo& Entry)
	{
		return DialogueInfoId == Entry.Id;
	});

	if (!MatchingInfo)
	{
		FChronicle_DialogueInfo NewInfo;
		NewInfo.Id = DialogueInfoId;

		if (SequenceInfo.bIsEntrySequence)
		{
			NewInfo.EntrySequenceId = SequenceInfo.Id;
		}
		
		Infos.Add(NewInfo);
		
		MatchingInfo = &Infos.Last();
	}

	MatchingInfo->SequencesById.Remove(SequenceInfo.Id);
	MatchingInfo->SequencesById.Add(SequenceInfo.Id, SequenceInfo);
}

void UChronicle_ExportInfo::UpdateStatus(const FString& Path, const EChronicle_CinematicAssetExportStatus Status)
{
	FChronicle_CinematicEntry* MatchingEntry = Entries.FindByPredicate([Path](const FChronicle_CinematicEntry& Entry)
	{
		return Entry.MainPath == Path;
	});

	if (!MatchingEntry)
	{
		return;
	}

	MatchingEntry->Status = Status;
}

void UChronicle_ExportInfo::UpdateExportInfo(const FString& Path, const FDateTime ExportTime, const FString& CinematicPath)
{
	FChronicle_CinematicEntry* MatchingEntry = Entries.FindByPredicate([Path](const FChronicle_CinematicEntry& Entry)
	{
		return Entry.MainPath == Path;
	});

	if (!MatchingEntry)
	{
		return;
	}

	MatchingEntry->CinematicPath = CinematicPath;
	MatchingEntry->LastExportTime = ExportTime;
}

FChronicle_CinematicEntry UChronicle_ExportInfo::TryAdd(const FChronicle_CinematicEntry& InEntry)
{
	const FChronicle_CinematicEntry* MatchingEntry = Entries.FindByPredicate([InEntry](const FChronicle_CinematicEntry& Entry)
	{
		return Entry.MainPath == InEntry.MainPath;
	});

	if (MatchingEntry)
	{
		return *MatchingEntry;
	}

	Entries.Add(InEntry);
	return InEntry;
}

void UChronicle_ExportInfo::Remove(const FString& Path)
{
	Entries.RemoveAll([&Path](const FChronicle_CinematicEntry& Entry)
	{
		return Entry.MainPath == Path;
	});
}

bool UChronicle_ExportInfo::AreEqual(const UChronicle_CinematicData* A, const UChronicle_CinematicData* B)
{
	if (!A || !B)
	{
		return A == B;
	}

	if (A->LineNodeIds != B->LineNodeIds)
	{
		return false;
	}

	if (A->SequencesData.Num() != B->SequencesData.Num())
	{
		return false;
	}

	for (int32 I = 0; I < A->SequencesData.Num(); I++)
	{
		const TArray<FChronicle_DialogueNodeData>& NodesA = A->SequencesData[I].Nodes;
		const TArray<FChronicle_DialogueNodeData>& NodesB = B->SequencesData[I].Nodes;

		if (NodesA.Num() != NodesB.Num())
		{
			return false;
		}

		for (int32 J = 0; J < NodesA.Num(); J++)
		{
			const FChronicle_DialogueNodeData& NodeA = NodesA[J];
			const FChronicle_DialogueNodeData& NodeB = NodesB[J];

			if (NodeA.Id != NodeB.Id
				|| NodeA.Type != NodeB.Type
				|| NodeA.Text != NodeB.Text
				|| NodeA.SpeakerId != NodeB.SpeakerId
				|| NodeA.ListenerId != NodeB.ListenerId
				|| NodeA.Callbacks != NodeB.Callbacks
				|| NodeA.Children != NodeB.Children
			)
			{
				return false;
			}

			for (int32 K = 0; K < NodeA.Rules.Num(); K++)
			{
				const FChronicle_RuleData& RuleA = NodeA.Rules[K];
				const FChronicle_RuleData& RuleB = NodeB.Rules[K];

				if (RuleA.Id != RuleB.Id
					|| RuleA.Type != RuleB.Type
					|| RuleA.ParameterType != RuleB.ParameterType
					|| RuleA.ParameterCharacterId != RuleB.ParameterCharacterId
					|| RuleA.ParameterInteger != RuleB.ParameterInteger
					|| RuleA.Input != RuleB.Input
					|| RuleA.Output != RuleB.Output
				)
				{
					return false;
				}
			}
		}
	}

	return true;
}
