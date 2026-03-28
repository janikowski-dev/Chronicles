#include "FChronicle_RuleSystemEditorModule.h"

#include "FChronicle_RuleDirectory.h"

#define LOCTEXT_NAMESPACE "FRuleSystemModule"

void FChronicle_RuleSystemEditorModule::StartupModule()
{
	FChronicle_RuleDirectory::Refresh();
}

void FChronicle_RuleSystemEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FChronicle_RuleSystemEditorModule, RuleSystemEditor)