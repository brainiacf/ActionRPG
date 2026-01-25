
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionRPGProjectileBase.generated.h"


class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

//UClass (Abstract) marks the class abstract, meaning it cannot be instantiated directly - only subclasses derived from it 
UCLASS(Abstract)
class ACTIONRPG_API AActionRPGProjectileBase : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,Category=Effects)
	TObjectPtr<UParticleSystem> ImpactVFX;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Components)
	TObjectPtr<USphereComponent> SphereComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Components)
	TObjectPtr<UProjectileMovementComponent> MovementComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Components)
	TObjectPtr<UParticleSystemComponent> EffectComponent;
	
	//func
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent*HitComponent,AActor*OtherActor,UPrimitiveComponent*OtherComp,FVector NormalImpulse,const FHitResult &HitResult);
	
	// Blueprint Native event -> CPP base implementation can be expanded in Blueprint
	// Blueprint Callable -> to allow child classes trigger explosion
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void Explode();
	
	virtual void PostInitializeComponents() override;
	
public:	
	AActionRPGProjectileBase();


};
