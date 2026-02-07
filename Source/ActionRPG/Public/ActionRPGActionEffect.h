
#pragma once

#include "CoreMinimal.h"
#include "ActionRPGAction.h"
#include "ActionRPGActionEffect.generated.h"

/**
 * I'm gonna fight 'em off
 * A Seven nation army couldn't hold me back
 */
UCLASS()
class ACTIONRPG_API UActionRPGActionEffect : public UActionRPGAction
{
	GENERATED_BODY()
public:
	void StartAction_Implementation(AActor* Instigator) override;
	void StopAction_Implementation(AActor* Instigator) override;
protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Effects")
	float Duration;
	
	// Time between 'ticks' to apply effect
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Effects")
	float Period;
	
	FTimerHandle Period_Handle;
	FTimerHandle Duration_Handle;
	
	UFUNCTION(BlueprintNativeEvent, Category="Effects")
	void ExecutePeriodicEffect(AActor *Instigator);
	
public:
	UActionRPGActionEffect();
	
};
