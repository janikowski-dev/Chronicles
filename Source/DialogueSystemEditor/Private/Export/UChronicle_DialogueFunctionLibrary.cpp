#include "UChronicle_DialogueFunctionLibrary.h"
#include "FChronicle_DialogueExporter.h"

UChronicle_DialogueData* UChronicle_DialogueFunctionLibrary::ConvertToData(const UChronicle_DialogueAsset* Asset)
{
	return FChronicle_DialogueExporter::ConvertToData(Asset);
}

FString UChronicle_DialogueFunctionLibrary::ConvertToJson(const UChronicle_DialogueAsset* Asset)
{
	return FChronicle_DialogueExporter::ConvertToJson(Asset);
}
