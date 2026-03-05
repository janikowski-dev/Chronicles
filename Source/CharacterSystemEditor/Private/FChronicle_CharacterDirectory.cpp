#include "FChronicle_CharacterDirectory.h"

#include "FChronicle_CombinedCharacterSet.h"
#include "UChronicle_CharacterSettings.h"

FChronicle_CombinedCharacterSet FChronicle_CharacterDirectory::AllCharacterSet(
	PlayableCharacterSet,
	NonPlayableCharacterSet
);
FChronicle_CharacterSet FChronicle_CharacterDirectory::NonPlayableCharacterSet;
FChronicle_CharacterSet FChronicle_CharacterDirectory::PlayableCharacterSet;

FChronicle_CombinedCharacterSet& FChronicle_CharacterDirectory::GetAll()
{
	return AllCharacterSet;
}

FChronicle_CharacterSet& FChronicle_CharacterDirectory::GetPlayable()
{
	return PlayableCharacterSet;
}

FChronicle_CharacterSet& FChronicle_CharacterDirectory::GetNonPlayable()
{
	return NonPlayableCharacterSet;
}

void FChronicle_CharacterDirectory::Refresh()
{
	const UChronicle_CharacterSettings* const Settings = GetDefault<UChronicle_CharacterSettings>();
	NonPlayableCharacterSet.Refresh(Settings->NonPlayableCharacters);
	PlayableCharacterSet.Refresh(Settings->PlayableCharacters);
}
