
#pragma once

#include "CoreMinimal.h"
#include "ActionRPGPowerupActor.h"
#include "ActionRPGPowerUp_Action.generated.h"

/**
 * 
 */
class UActionRPGAction;
class UActionRPGActionComponent;
UCLASS()
class ACTIONRPG_API AActionRPGPowerUp_Action : public AActionRPGPowerupActor
{
	GENERATED_BODY()
	
public:
	void Interact_Implementation(APawn* InstigatorPawn) override;
protected:
	UPROPERTY(EditAnywhere,Category=PowerUp)
	TSubclassOf<UActionRPGAction> ActionToGrant;
};
