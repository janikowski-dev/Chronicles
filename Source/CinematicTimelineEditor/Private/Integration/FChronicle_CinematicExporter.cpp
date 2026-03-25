#include "FChronicle_CinematicExporter.h"

#include "FileHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"

UChronicle_CinematicData* FChronicle_CinematicExporter::ConvertToCinematicData(const UChronicle_DialogueData* Input)
{
    UChronicle_CinematicData* Output = NewObject<UChronicle_CinematicData>();
    PopulateOutput(Input, Output);
    return Output;
}

UChronicle_CinematicData* FChronicle_CinematicExporter::ExportToCinematicData(const UChronicle_DialogueData* Input, const FString Path)
{
    UPackage* Package = CreatePackage(*Path);
    Package->FullyLoad();

    UChronicle_CinematicData* Output = NewObject<UChronicle_CinematicData>(
        Package,
        UChronicle_CinematicData::StaticClass(),
        *FPaths::GetBaseFilename(Path),
        RF_Public | RF_Standalone
    );

    PopulateOutput(Input, Output);

    FString PackageFilename;
    FPackageName::TryConvertLongPackageNameToFilename(Path,PackageFilename,FPackageName::GetAssetPackageExtension());

    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
    SaveArgs.Error = GError;
    SaveArgs.bForceByteSwapping = false;
    SaveArgs.bWarnOfLongFilename = true;

    UPackage::SavePackage(Package, Output, *PackageFilename, SaveArgs);
    UEditorLoadingAndSavingUtils::SavePackages({ Package }, false);
    FAssetRegistryModule::AssetCreated(Output);
    return Output;
}

void FChronicle_CinematicExporter::PopulateOutput(const UChronicle_DialogueData* Input, UChronicle_CinematicData* Output)
{
    TMap<FGuid, const FChronicle_DialogueNodeData*> NodeMap;
    NodeMap.Reserve(Input->Nodes.Num());

    for (const FChronicle_DialogueNodeData& Node : Input->Nodes)
    {
        NodeMap.Add(Node.Id, &Node);
    }
    
    Output->ParticipantIds = Input->ParticipantIds;

    TMap<FGuid, FGuid> FirstNodeToSequenceId;
    TMap<FGuid, FChronicle_SequenceData> SequenceMap;

    for (const FChronicle_DialogueNodeData& Node : Input->Nodes)
    {
        if (Node.Type == EChronicle_DialogueNodeType::Line)
        {
            FirstNodeToSequenceId.Add(Node.Id, FGuid::NewGuid());
        }
    }

    const FChronicle_DialogueNodeData* Root = nullptr;

    for (const FChronicle_DialogueNodeData& Node : Input->Nodes)
    {
        if (Node.Type == EChronicle_DialogueNodeType::Root)
        {
            Root = &Node;
            break;
        }
    }

    if (!Root)
    {
        return;
    }

    struct FTraversalState
    {
        const FChronicle_DialogueNodeData* Node;
        TArray<FChronicle_DialogueNodeData> AccumulatedNodes;
        FGuid ParentSequenceId;
        bool bIsBranch;
    };

    TMap<FGuid, FGuid> SequenceIdToFirstNodeId;

    auto WireToParent = [&](const FGuid& FirstNodeId, const FGuid& ParentSequenceId, bool bIsBranch)
    {
        if (!ParentSequenceId.IsValid() || !FirstNodeId.IsValid())
        {
            return;
        }

        if (FChronicle_SequenceData* Parent = SequenceMap.Find(ParentSequenceId))
        {
            if (bIsBranch)
            {
                Parent->BranchSequenceIds.AddUnique(*FirstNodeToSequenceId.Find(FirstNodeId));
            }
            else
            {
                Parent->NextNodeId = FirstNodeId;
            }
        }
    };

    auto FlushSequence = [&](
        TArray<FChronicle_DialogueNodeData>& Nodes,
        const FGuid& ParentSequenceId,
        bool bIsBranch
    ) -> FGuid
    {
        if (Nodes.Num() == 0)
        {
            return FGuid();
        }

        const FGuid FirstNodeId = Nodes[0].Id;
        const FGuid SequenceId = FirstNodeToSequenceId.FindOrAdd(FirstNodeId, FGuid::NewGuid());

        FChronicle_SequenceData NewSequence;
        NewSequence.Id = SequenceId;
        NewSequence.Nodes = MoveTemp(Nodes);
        Nodes.Reset();

        SequenceMap.Add(SequenceId, MoveTemp(NewSequence));
        WireToParent(FirstNodeId, ParentSequenceId, bIsBranch);

        return SequenceId;
    };

    auto TryPushChild = [&](
        TArray<FTraversalState>& Stack,
        const FGuid& ChildId,
        TArray<FChronicle_DialogueNodeData> Accumulated,
        const FGuid& ParentSequenceId,
        bool bIsBranch
    )
    {
        const FChronicle_DialogueNodeData* const* Child = NodeMap.Find(ChildId);

        if (!Child)
        {
            return;
        }

        if (FGuid* ExistingSeqId = FirstNodeToSequenceId.Find(ChildId))
        {
            if (SequenceMap.Contains(*ExistingSeqId))
            {
                if (Accumulated.Num() > 0)
                {
                    FGuid FlushedId = FlushSequence(Accumulated, ParentSequenceId, bIsBranch);
                    if (FChronicle_SequenceData* Flushed = SequenceMap.Find(FlushedId))
                    {
                        Flushed->NextNodeId = ChildId;
                    }
                }
                else
                {
                    WireToParent(ChildId, ParentSequenceId, bIsBranch);
                }
                return;
            }
        }

        Stack.Push({ *Child, MoveTemp(Accumulated), ParentSequenceId, bIsBranch });
    };

    TArray<FTraversalState> Stack;
    Stack.Reserve(Input->Nodes.Num());
    Stack.Push({ Root, {}, FGuid(), false });

    while (Stack.Num() > 0)
    {
        FTraversalState State = Stack.Pop();
        const FChronicle_DialogueNodeData* Current = State.Node;

        if (!Current)
        {
            FlushSequence(State.AccumulatedNodes, State.ParentSequenceId, State.bIsBranch);
            continue;
        }

        switch (Current->Type)
        {
        case EChronicle_DialogueNodeType::Root:
        {
            for (const FGuid& ChildId : Current->Children)
            {
                TryPushChild(Stack, ChildId, {}, FGuid(), false);
            }
            break;
        }

        case EChronicle_DialogueNodeType::Link:
        {
            if (Current->LinkTargetId.IsValid())
            {
                FGuid FlushedId = FlushSequence(State.AccumulatedNodes, State.ParentSequenceId, State.bIsBranch);
                
                if (FChronicle_SequenceData* Flushed = SequenceMap.Find(FlushedId))
                {
                    Flushed->NextNodeId = Current->LinkTargetId;
                }
            }
            else
            {
                FlushSequence(State.AccumulatedNodes, State.ParentSequenceId, State.bIsBranch);
            }
            break;
        }

        case EChronicle_DialogueNodeType::Response:
        case EChronicle_DialogueNodeType::Line:
        {
            State.AccumulatedNodes.Add(*Current);
            Output->LineNodeIds.Add(Current->Id);

            if (Current->Children.Num() == 0)
            {
                FlushSequence(State.AccumulatedNodes, State.ParentSequenceId, State.bIsBranch);
            }
            else if (Current->Children.Num() == 1)
            {
                TryPushChild(Stack, Current->Children[0], MoveTemp(State.AccumulatedNodes), State.ParentSequenceId, State.bIsBranch);
            }
            else
            {
                TArray<FGuid> LineChildren;
                TArray<FGuid> OtherChildren;

                for (const FGuid& ChildId : Current->Children)
                {
                    if (const FChronicle_DialogueNodeData* const* Child = NodeMap.Find(ChildId))
                    {
                        if ((*Child)->Type == EChronicle_DialogueNodeType::Line)
                        {
                            LineChildren.Add(ChildId);
                        }
                        else
                        {
                            OtherChildren.Add(ChildId);
                        }
                    }
                }

                FGuid FlushedId = FlushSequence(State.AccumulatedNodes, State.ParentSequenceId, State.bIsBranch);

                for (const FGuid& ChildId : LineChildren)
                {
                    TryPushChild(Stack, ChildId, {}, FlushedId, false);
                }

                for (const FGuid& ChildId : OtherChildren)
                {
                    TryPushChild(Stack, ChildId, {}, FlushedId, true);
                }
            }
            break;
        }

        default:
        {
            for (const FGuid& ChildId : Current->Children)
            {
                TryPushChild(Stack, ChildId, State.AccumulatedNodes, State.ParentSequenceId, State.bIsBranch);
            }
            break;
        }
        }
    }

    for (auto& Pair : SequenceMap)
    {
        Output->SequencesData.Add(MoveTemp(Pair.Value));
    }
}
