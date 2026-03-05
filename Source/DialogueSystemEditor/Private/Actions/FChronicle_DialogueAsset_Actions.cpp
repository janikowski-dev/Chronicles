#include "FChronicle_DialogueAsset_Actions.h"
#include "Assets/UChronicle_DialogueAsset.h"
#include "Editors/FChronicle_DialogueEditor.h"
#include "Export/FChronicle_DialogueExporter.h"

FText FChronicle_DialogueAsset_Actions::GetName() const
{
	return FText::FromString("Dialogue Asset");
}

FColor FChronicle_DialogueAsset_Actions::GetTypeColor() const
{
	return FColor::Purple;
}

UClass* FChronicle_DialogueAsset_Actions::GetSupportedClass() const
{
	return UChronicle_DialogueAsset::StaticClass();
}

uint32 FChronicle_DialogueAsset_Actions::GetCategories()
{
	return FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get().FindAdvancedAssetCategory(FName("Chronicle"));
}

void FChronicle_DialogueAsset_Actions::OpenAssetEditor(
	const TArray<UObject*>& InObjects,
	const TSharedPtr<IToolkitHost> EditWithinLevelEditor
)
{
	for (UObject* Object : InObjects)
	{
		UChronicle_DialogueAsset* Asset = Cast<UChronicle_DialogueAsset>(Object);
		
		if (!Asset)
		{
			continue;
		}
		
		const TSharedRef<FChronicle_DialogueEditor> Editor = MakeShared<FChronicle_DialogueEditor>();

		Editor->InitDialogueAssetEditor(
			EToolkitMode::Standalone,
			EditWithinLevelEditor,
			Asset
		);
	}
}

void FChronicle_DialogueAsset_Actions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString("Export to runtime UObject"),
		FText::FromString("Export dialogue to runtime asset"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Save"),
		FUIAction(FExecuteAction::CreateLambda([InObjects]
		{
			for (UObject* Object : InObjects)
			{
				if (const UChronicle_DialogueAsset* Asset = Cast<UChronicle_DialogueAsset>(Object))
				{
					FChronicle_DialogueExporter::ExportToAsset(Asset);
				}
			}
		}))
	);
	
	MenuBuilder.AddMenuEntry(
		FText::FromString("Copy JSON to clipboard"),
		FText::FromString("Copies asset as JSON to clipboard"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Clipboard"),
		FUIAction(FExecuteAction::CreateLambda([InObjects]
		{
			for (UObject* Object : InObjects)
			{
				if (const UChronicle_DialogueAsset* Asset = Cast<UChronicle_DialogueAsset>(Object))
				{
					FChronicle_DialogueExporter::CopyToClipboard(Asset);
				}
			}
		}))
	);
}
