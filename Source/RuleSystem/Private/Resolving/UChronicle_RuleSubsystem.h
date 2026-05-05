#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UChronicle_RuleSubsystem.generated.h"

struct FChronicle_CallbackData;
struct FChronicle_RuleData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChronicle_RuleCharacterIdDelegate, FGuid, RuleId, FGuid, CharacterId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChronicle_RuleIntegerDelegate, FGuid, RuleId, int, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChronicle_RuleRawDelegate, FGuid, RuleId);

UCLASS()
class RULESYSTEM_API UChronicle_RuleSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FChronicle_RuleCharacterIdDelegate OnCharacterIdCallbackCalled;

	UPROPERTY(BlueprintAssignable)
	FChronicle_RuleIntegerDelegate OnIntegerCallbackCalled;

	UPROPERTY(BlueprintAssignable)
	FChronicle_RuleRawDelegate OnRawCallbackCalled;
	
private:
	UPROPERTY()
	TMap<FGuid, bool> BoolRules;
	
	UPROPERTY()
	TMap<FGuid, int> IntegerRules;
	
	UPROPERTY()
	TMap<FGuid, FGuid> CharacterIdRules;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category="Chronicle|Rules")
	bool Resolve(TArray<FChronicle_RuleData> Rules);

	UFUNCTION(BlueprintCallable, Category="Chronicle|Rules")
	void Raise(TArray<FChronicle_CallbackData> Callbacks);

	UFUNCTION(BlueprintCallable, Category="Chronicle|Rules")
	void SetBool(FGuid RuleId, bool bValue);

	UFUNCTION(BlueprintCallable, Category="Chronicle|Rules")
	void SetInteger(FGuid RuleId, int Value);

	UFUNCTION(BlueprintCallable, Category="Chronicle|Rules")
	void SetCharacterId(FGuid RuleId, FGuid CharacterId);
	
private:
	void EmptyRules();
};