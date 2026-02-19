
#pragma once

#include "CoreMinimal.h"
#include "ActionRPGProjectileBase.h"
#include  "GameplayTags.h"
#include "ActionRPGMagicProjectile.generated.h"

class UActionRPGActionEffect;

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AActionRPGMagicProjectile : public AActionRPGProjectileBase
{
	GENERATED_BODY()
public:
	AActionRPGMagicProjectile();
	
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(EditDefaultsOnly,Category=Damage)
	TSubclassOf<UActionRPGActionEffect> BurningActionClass; 
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnActorHit(UPrimitiveComponent *HitComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComponent,
		FVector NormalImpulse,
		const FHitResult &HitResult) override;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly,Category=Damage)
	float DamageAmount;
	
	UPROPERTY(EditDefaultsOnly,Category=Rage)
	float RageAmount;
	
	UPROPERTY(EditDefaultsOnly,Category=Damage)
	FGameplayTag ParryTag;
	
};
