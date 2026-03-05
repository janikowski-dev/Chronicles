#include "FRuleSystemEditorModule.h"

#include "FChronicle_RuleDirectory.h"

#define LOCTEXT_NAMESPACE "FRuleSystemModule"

void FRuleSystemEditorModule::StartupModule()
{
	FChronicle_RuleDirectory::Refresh();
}

void FRuleSystemEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRuleSystemEditorModule, RuleSystemEditor)