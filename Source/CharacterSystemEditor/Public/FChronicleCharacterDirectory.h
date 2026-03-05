#pragma once

#include "FCharacterSet.h"

class CHARACTERSYSTEMEDITOR_API FChronicleCharacterDirectory
{
public:
	static FCharacterSet& GetNonPlayable();
	static FCharacterSet& GetPlayable();
	static FCharacterSet& GetAll();
	static void Refresh();

private:
	static FCharacterSet NonPlayableCharacterSet;
	static FCharacterSet PlayableCharacterSet;
	static FCharacterSet AllCharacterSet;
};
