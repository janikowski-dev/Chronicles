#pragma once

#include "Data/UChronicle_CinematicData.h"
#include "Data/UChronicle_DialogueData.h"

class FChronicle_CinematicExporter
{
public:
	static UChronicle_CinematicData* ExportToCinematicData(const UChronicle_DialogueData* Input, const FString Path);
	static UChronicle_CinematicData* ConvertToCinematicData(const UChronicle_DialogueData* Input);

private:
	static void PopulateOutput(const UChronicle_DialogueData* Input, UChronicle_CinematicData* Output);
};
