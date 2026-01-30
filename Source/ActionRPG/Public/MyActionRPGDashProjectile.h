
#pragma once

#include "CoreMinimal.h"
#include "ActionRPGProjectileBase.h"
#include "MyActionRPGDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AMyActionRPGDashProjectile : public AActionRPGProjectileBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,Category=Teleport)
	float TeleportDelay;
	UPROPERTY(EditDefaultsOnly,Category=Teleport)
	float DetonateDelay;
	//Handle to cancel timer if we already Hit Something
	FTimerHandle TimerHandle_DelayDetonate;
	
	virtual void Explode_Implementation() override;
	void TeleportInstigator();
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:
	AMyActionRPGDashProjectile();
	
	

};
