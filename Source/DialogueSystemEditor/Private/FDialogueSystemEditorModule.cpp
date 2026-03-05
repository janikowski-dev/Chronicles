#include "FDialogueSystemEditorModule.h"

#include "AssetToolsModule.h"
#include "EdGraphUtilities.h"
#include "Actions/FChronicle_DialogueAsset_Actions.h"
#include "Factories/FChronicle_RuleNodeFactory.h"
#include "Factories/FChronicle_DialogueNodeFactory.h"
#include "Utils/FChronicle_EditorStyle.h"

#define LOCTEXT_NAMESPACE "FDialogueSystemModule"

void FDialogueSystemEditorModule::StartupModule()
{
	FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get().RegisterAdvancedAssetCategory(
		FName("Chronicle"),
		FText::FromString("Chronicle")
	);
	FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get().RegisterAssetTypeActions(
		MakeShared<FChronicle_DialogueAsset_Actions>()
	);
	
	FEdGraphUtilities::RegisterVisualNodeFactory(MakeShared<FChronicle_DialogueNodeFactory>());
	FEdGraphUtilities::RegisterVisualNodeFactory(MakeShared<FChronicle_RuleNodeFactory>());
	
	FChronicle_EditorStyle::Initialize();
}

void FDialogueSystemEditorModule::ShutdownModule()
{
	FChronicle_EditorStyle::Shutdown();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogueSystemEditorModule, DialogueSystemEditor)