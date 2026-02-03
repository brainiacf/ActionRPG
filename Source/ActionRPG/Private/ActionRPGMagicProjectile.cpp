#include "ActionRPGMagicProjectile.h"
#include "ActionRPGAttributeComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ActionRPGGameplayInterface.h"
#include "ActionRPGActionComponent.h"
#include "ActionRPGGameplayFunctionLibrary.h"
#include "Engine/Engine.h"


AActionRPGMagicProjectile::AActionRPGMagicProjectile()
{
	DamageAmount = 20.0f;
	//SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnActorHit);
	
}

void AActionRPGMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnActorOverlap);
}

void AActionRPGMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(5,5.0,FColor::Green,TEXT("Hit Damage"));
	if (OtherActor && OtherActor != GetInstigator())
	{
		
		
		UActionRPGActionComponent* ActionComponent = Cast<UActionRPGActionComponent>(OtherActor->GetComponentByClass(UActionRPGActionComponent::StaticClass()));
		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComponent->Velocity = -MovementComponent->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		if (UActionRPGGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(),OtherActor,DamageAmount,SweepResult))
		{
			Explode();
		}
	}
}

void AActionRPGMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& HitResult)
{
	
	//Super::OnActorHit(HitComponent, OtherActor, OtherComponent, NormalImpulse, HitResult);

	
	
}

void AActionRPGMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(),true);
}
