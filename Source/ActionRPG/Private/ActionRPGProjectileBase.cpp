

#include "ActionRPGProjectileBase.h"

#include "Components/SphereComponent.h"
#include "ActionRPGAttributeComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/FastReferenceCollector.h"
#include "GameFramework/Actor.h"


AActionRPGProjectileBase::AActionRPGProjectileBase()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;
	
	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComp"));
	EffectComponent->SetupAttachment(RootComponent);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->bRotationFollowsVelocity = true;//controls whether each particle's rotation aligns with its velocity vector
	MovementComponent->bInitialVelocityInLocalSpace = true; // defines which space the particle's initial velocity is interpreted in: true ini. vel. is relative to the emitter's local rotation and pos. false -> ini. vel. is in world space.
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->InitialSpeed = 6000.0f;
}

void AActionRPGProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	Explode();
}

void AActionRPGProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->SetCollisionProfileName(TEXT("Projectile"));
	SphereComponent->SetNotifyRigidBodyCollision(true); //"Simulation generates Hit Events"
	SphereComponent->SetGenerateOverlapEvents(false);
}

void AActionRPGProjectileBase::Explode_Implementation()
{
	// I think this is not the correct wat to initialize projectile also to destroy this// will work on optimization. 
	
	// check to make sure we aren't getting already being destroyed 
	// adding ensure to see if we encounter this situation at all 
	UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());
	Destroy();
}
