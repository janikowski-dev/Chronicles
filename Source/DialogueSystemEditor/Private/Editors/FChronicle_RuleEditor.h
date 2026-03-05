#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/IToolkitHost.h"

class UChronicle_DialogueNode;

class DIALOGUESYSTEMEDITOR_API FChronicle_RuleEditor : public FAssetEditorToolkit
{
public:
	void InitNodeAssetEditor(
		const EToolkitMode::Type Mode,
		const TSharedPtr<IToolkitHost>& InitToolkitHost,
		UChronicle_DialogueNode* Node
	);
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

private:
	TSharedRef<SDockTab> SpawnGraphTab(const FSpawnTabArgs& Args);
	TArray<UObject*> Cache(UChronicle_DialogueNode* Node);
	void DeleteSelectedNodes() const;
	bool CanDeleteNodes() const;

private:
	TSharedPtr<FUICommandList> GraphEditorCommands;
	TSharedPtr<SGraphEditor> GraphEditor;
	UChronicle_DialogueNode* EditedNode = nullptr;
};
