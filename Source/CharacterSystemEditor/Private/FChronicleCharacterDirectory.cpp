#include "FChronicleCharacterDirectory.h"

#include "UChronicleCharacterSettings.h"

FCharacterSet FChronicleCharacterDirectory::PlayableCharacterSet;
FCharacterSet FChronicleCharacterDirectory::AllCharacterSet;
FCharacterSet FChronicleCharacterDirectory::NonPlayableCharacterSet;

FCharacterSet& FChronicleCharacterDirectory::GetPlayable()
{
	return PlayableCharacterSet;
}

FCharacterSet& FChronicleCharacterDirectory::GetNonPlayable()
{
	return NonPlayableCharacterSet;
}

FCharacterSet& FChronicleCharacterDirectory::GetAll()
{
	return AllCharacterSet;
}

void FChronicleCharacterDirectory::Refresh()
{
	const UChronicleCharacterSettings* const Settings = GetDefault<UChronicleCharacterSettings>();
	NonPlayableCharacterSet.Refresh(Settings->NonPlayableCharacters);
	PlayableCharacterSet.Refresh(Settings->PlayableCharacters);
	AllCharacterSet.Refresh(Settings->NonPlayableCharacters);
	AllCharacterSet.Refresh(Settings->PlayableCharacters);
}
