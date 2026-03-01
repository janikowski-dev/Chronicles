#include "FRuleSystemEditorModule.h"

#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "FRuleSystemModule"

void FRuleSystemEditorModule::StartupModule()
{
	FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get().RegisterAdvancedAssetCategory(
		FName("Chronicle"),
		FText::FromString("Chronicle")
	);
}

void FRuleSystemEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRuleSystemEditorModule, RuleSystemEditor)