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
    };

    auto FlushSequence = [&](TArray<FChronicle_DialogueNodeData>& Nodes)
    {
        if (Nodes.Num() == 0)
        {
            return;
        }

        FChronicle_SequenceData NewSequence;
        NewSequence.Nodes = MoveTemp(Nodes);
        Output->SequencesData.Add(MoveTemp(NewSequence));
        Nodes.Reset();
    };

    auto PushChildren = [&](TArray<FTraversalState>& Stack, const FChronicle_DialogueNodeData* Node, TArray<FChronicle_DialogueNodeData> SharedAccumulated)
    {
        for (const FGuid& ChildId : Node->Children)
        {
            if (const FChronicle_DialogueNodeData* const* Child = NodeMap.Find(ChildId))
            {
                Stack.Push({ *Child, SharedAccumulated });
            }
        }
    };

    TArray<FTraversalState> Stack;
    TSet<FGuid> Visited;

    Stack.Reserve(Input->Nodes.Num());
    Stack.Push({ Root, {} });

    while (Stack.Num() > 0)
    {
        FTraversalState State = Stack.Pop();
        const FChronicle_DialogueNodeData* Current = State.Node;

        if (!Current || Visited.Contains(Current->Id))
        {
            FlushSequence(State.AccumulatedNodes);
            continue;
        }

        Visited.Add(Current->Id);
        State.AccumulatedNodes.Add(*Current);

        switch (Current->Type)
        {
        case EChronicle_DialogueNodeType::Root:
            {
                PushChildren(Stack, Current, MoveTemp(State.AccumulatedNodes));
                break;
            }

        case EChronicle_DialogueNodeType::Line:
            {
                Output->LineNodeIds.Add(Current->Id);

                if (Current->Children.Num() == 0)
                {
                    FlushSequence(State.AccumulatedNodes);
                }
                else if (Current->Children.Num() == 1)
                {
                    if (const FChronicle_DialogueNodeData* const* Child = NodeMap.Find(Current->Children[0]))
                    {
                        Stack.Push({ *Child, MoveTemp(State.AccumulatedNodes) });
                    }
                }
                else
                {
                    FlushSequence(State.AccumulatedNodes);
                    PushChildren(Stack, Current, {});
                }
                break;
            }

        default:
            {
                FlushSequence(State.AccumulatedNodes);
                PushChildren(Stack, Current, {});
                break;
            }
        }
    }
}
