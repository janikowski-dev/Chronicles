#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/IToolkitHost.h"

class UChronicle_DialogueAsset;

class DIALOGUESYSTEMEDITOR_API FChronicle_DialogueEditor : public FAssetEditorToolkit
{
public:
	void InitDialogueAssetEditor(
		const EToolkitMode::Type Mode,
		const TSharedPtr<IToolkitHost>& InitToolkitHost,
		UChronicle_DialogueAsset* Asset
	);
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

private:
	TSharedRef<SDockTab> SpawnGraphTab(const FSpawnTabArgs& Args);
	TArray<UObject*> Cache(UChronicle_DialogueAsset* Asset);

public:
	TSharedPtr<SGraphEditor> GraphEditor;

private:
	UChronicle_DialogueAsset* EditedAsset = nullptr;
};