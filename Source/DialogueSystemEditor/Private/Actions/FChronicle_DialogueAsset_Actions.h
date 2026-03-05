#pragma once

#include "AssetTypeActions_Base.h"

class FChronicle_DialogueAsset_Actions : public FAssetTypeActions_Base
{
public:
	virtual void OpenAssetEditor(
		const TArray<UObject*>& InObjects,
		TSharedPtr<IToolkitHost> EditWithinLevelEditor
	) override;

	virtual void GetActions(
		const TArray<UObject*>& InObjects,
		FMenuBuilder& MenuBuilder
	) override;
	
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
};
