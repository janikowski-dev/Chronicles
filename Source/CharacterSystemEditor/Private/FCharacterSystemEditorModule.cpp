#include "FCharacterSystemEditorModule.h"

#include "FChronicle_CharacterDirectory.h"

#define LOCTEXT_NAMESPACE "FCharacterSystemModule"

void FCharacterSystemEditorModule::StartupModule()
{
	FChronicle_CharacterDirectory::Refresh();
}

void FCharacterSystemEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCharacterSystemEditorModule, CharacterSystemEditor)