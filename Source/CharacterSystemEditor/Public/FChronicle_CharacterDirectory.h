#pragma once

#include "FChronicle_CharacterSet.h" 
#include "FChronicle_CombinedCharacterSet.h"

class CHARACTERSYSTEMEDITOR_API FChronicle_CharacterDirectory
{
public:
	static FChronicle_CombinedCharacterSet& GetAll();
	static FChronicle_CharacterSet& GetNonPlayable();
	static FChronicle_CharacterSet& GetPlayable();
	static void Refresh();

private:
	static FChronicle_CombinedCharacterSet AllCharacterSet;
	static FChronicle_CharacterSet NonPlayableCharacterSet;
	static FChronicle_CharacterSet PlayableCharacterSet;
};
