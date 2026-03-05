#include "UChronicle_DialogueAssetFactory.h"
#include "Assets/UChronicle_DialogueAsset.h"
#include "Graphs/UChronicle_DialogueGraph.h"
#include "Schemas/UChronicle_DialogueGraphSchema.h"

UChronicle_DialogueAssetFactory::UChronicle_DialogueAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UChronicle_DialogueAsset::StaticClass();
}

UObject* UChronicle_DialogueAssetFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	const FName InName,
	const EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn
)
{
	UChronicle_DialogueAsset* Asset = NewObject<UChronicle_DialogueAsset>(
		InParent,
		InClass,
		InName,
		Flags | RF_Transactional
	);

	Asset->Graph = NewObject<UChronicle_DialogueGraph>(
		Asset,
		UChronicle_DialogueGraph::StaticClass(),
		NAME_None,
		RF_Transactional
	);

	Asset->Graph->Schema = UChronicle_DialogueGraphSchema::StaticClass();
	Asset->Graph->GetSchema()->CreateDefaultNodesForGraph(*Asset->Graph);
	Asset->Graph->Modify();
	Asset->Modify();

	return Asset;
}