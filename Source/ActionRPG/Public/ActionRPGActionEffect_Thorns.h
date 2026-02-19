#pragma once

#include "CoreMinimal.h"
#include "ActionRPGActionEffect.h"
#include "ActionRPGActionEffect_Thorns.generated.h"

/**
 * 
 */
class UActionRPGAttributeComponent;
UCLASS()
class ACTIONRPG_API UActionRPGActionEffect_Thorns : public UActionRPGActionEffect
{
	GENERATED_BODY()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	
	UActionRPGActionEffect_Thorns();
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
protected:
	
	UPROPERTY(EditDefaultsOnly,Category=Thorns)
	float ReflectFraction;
	
	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor,UActionRPGAttributeComponent* OwningComp,float NewHealth, float Delta);
	
	
	
};
