#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionRPGGameplayInterface.h"
#include "ActionRPGPowerupActor.generated.h"

class USphereComponent;

UCLASS()
class ACTIONRPG_API AActionRPGPowerupActor : public AActor, public IActionRPGGameplayInterface
{
	GENERATED_BODY()

public:
	AActionRPGPowerupActor();

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	
	UPROPERTY(EditDefaultsOnly,ReplicatedUsing=OnRep_IsActive, Category = Visibility)
	bool bIsActive;
	
	UFUNCTION()
	void OnRep_IsActive(bool bOldIsActive);
	
	UPROPERTY(EditDefaultsOnly, Category=PowerUp)
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UFUNCTION()
	void ShowPowerUp();

	void HideAndCoolDownPowerUp();

	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = Components)
	TObjectPtr<USphereComponent> SphereComponent;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                     UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                     const FHitResult& SweepResult);

	virtual void PostInitializeComponents() override;
};
