#include "FCharacterSystemEditorModule.h"

#include "FChronicleCharacterDirectory.h"

#define LOCTEXT_NAMESPACE "FCharacterSystemModule"

void FCharacterSystemEditorModule::StartupModule()
{
	FChronicleCharacterDirectory::Refresh();
}

void FCharacterSystemEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCharacterSystemEditorModule, CharacterSystemEditor)