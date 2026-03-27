#include "UChronicle_CoreFunctionLibrary.h"

#include "LevelSequencePlayer.h"

void UChronicle_CoreFunctionLibrary::OffsetSpawnableTransforms(
    ALevelSequenceActor* LevelSequenceActor,
    const FVector& LocationOffset,
    const FRotator& RotationOffset
)
{
    if (!LevelSequenceActor)
    {
        return;
    }

    ULevelSequence* LevelSequence = Cast<ULevelSequence>(LevelSequenceActor->GetSequence());
    
    if (!LevelSequence)
    {
        return;
    }

    UMovieScene* MovieScene = LevelSequence->GetMovieScene();
    
    if (!MovieScene)
    {
        return;
    }

    ULevelSequencePlayer* Player = LevelSequenceActor->GetSequencePlayer();
    
    if (!Player)
    {
        return;
    }

    for (int i = 0; i < MovieScene->GetSpawnableCount(); i++)
    {
        const FMovieSceneSpawnable& Spawnable = MovieScene->GetSpawnable(i);
        TArray<UObject*> BoundObjects = Player->GetBoundObjects(FMovieSceneObjectBindingID(Spawnable.GetGuid()));

        for (UObject* BoundObject : BoundObjects)
        {
            AActor* Actor = Cast<AActor>(BoundObject);
            
            if (!Actor)
            {
                continue;
            }

            Actor->SetActorLocationAndRotation(
                Actor->GetActorLocation() + LocationOffset,
                Actor->GetActorRotation() + RotationOffset
            );
        }
    }
}