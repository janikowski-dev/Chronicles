#pragma once

#include "ERuleParameterType.h"
#include "URuleNode.h"
#include "URuleInputNode.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API URuleInputNode : public URuleNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	ERuleParameterType ParameterType;
	
	UPROPERTY()
	FGuid RuleId;
	
	UPROPERTY()
	FGuid CharacterIdParameter;
	
	UPROPERTY()
	int32 IntegerParameter;
};
