
#pragma once

#include "CoreMinimal.h"
#include "ActionRPGAction.h"
#include "ActionRPGAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;

/**
 * Action that plays Animation and spawn projectile after a delay
 */
UCLASS()
class ACTIONRPG_API UActionRPGAction_ProjectileAttack : public UActionRPGAction
{
	GENERATED_BODY()
public:
	UActionRPGAction_ProjectileAttack();
	
	//Main Execution function called by the actionComponent
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
protected:
	
	// specific actor class to spawn (MagicProj,BlackHoleProj,DashProj)
	UPROPERTY(EditAnywhere,Category=Attack)
	TSubclassOf<AActor> ProjectileClass;
	
	//Time in seconds to wait between starting the animation and spawning the projectile 
	//This helps sync the projectile spawn with the character's hand movement
	UPROPERTY(EditDefaultsOnly,Category = Attack)
	float AttackAnimDelay;
	
	//The Animation montage to play on the character 
	UPROPERTY(EditAnywhere,Category=Attack)
	TObjectPtr<UAnimMontage> AttackAnim;
	
	//The name of the socket where projectile will spawn 
	UPROPERTY(EditAnywhere,Category=Attack)
	FName HandSocketName;
	
	//Particle effect to play immediately when the action Starts(e.g. casting flare)
	UPROPERTY(EditAnywhere,Category=Effects)
	TObjectPtr<UParticleSystem> CastingEffects;
	
	//Timer callback function to spawn the projectile 
	UFUNCTION()
	void AttackAnimDelay_Elapsed(ACharacter *InstigatorCharacter);





};
