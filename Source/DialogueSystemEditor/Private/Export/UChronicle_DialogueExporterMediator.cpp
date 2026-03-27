#include "UChronicle_DialogueExporterMediator.h"
#include "FChronicle_DialogueExporter.h"

UChronicle_DialogueData* UChronicle_DialogueExporterMediator::ConvertToData(const UChronicle_DialogueAsset* Asset)
{
	return FChronicle_DialogueExporter::ConvertToData(Asset);
}

FString UChronicle_DialogueExporterMediator::ConvertToJson(const UChronicle_DialogueAsset* Asset)
{
	return FChronicle_DialogueExporter::ConvertToJson(Asset);
}
