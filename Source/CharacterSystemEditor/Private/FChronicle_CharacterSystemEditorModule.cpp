#include "FChronicle_CharacterSystemEditorModule.h"

#include "FChronicle_CharacterDirectory.h"

#define LOCTEXT_NAMESPACE "FCharacterSystemModule"

void FChronicle_CharacterSystemEditorModule::StartupModule()
{
	FChronicle_CharacterDirectory::Refresh();
}

void FChronicle_CharacterSystemEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FChronicle_CharacterSystemEditorModule, CharacterSystemEditor)