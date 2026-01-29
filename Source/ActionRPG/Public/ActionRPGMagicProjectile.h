
#pragma once

#include "CoreMinimal.h"
#include "ActionRPGProjectileBase.h"
#include  "GameplayTags.h"
#include "ActionRPGMagicProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AActionRPGMagicProjectile : public AActionRPGProjectileBase
{
	GENERATED_BODY()
public:
	AActionRPGMagicProjectile();
protected:
	virtual void OnActorHit(UPrimitiveComponent *HitComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComponent,
		FVector NormalImpulse,
		const FHitResult &HitResult) override;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly,Category=Damage)
	float DamageAmount;
	
	UPROPERTY(EditDefaultsOnly,Category=Damage)
	FGameplayTag ParryTag;
	
};
