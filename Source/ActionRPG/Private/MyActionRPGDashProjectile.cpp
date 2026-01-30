
#include "MyActionRPGDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"



void AMyActionRPGDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;
	MovementComponent->InitialSpeed = 6000.0f;
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	
}

AMyActionRPGDashProjectile::AMyActionRPGDashProjectile()
{
}


void AMyActionRPGDashProjectile::Explode_Implementation()
{
	// skipping super implementation as it will destroy actor ( we need to stay alive a bit longer to finish the 2nd timer)
	//Super::Explode_Implementation();
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayDetonate);
	UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());
	
	MovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);
	
	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport,this,&ThisClass::TeleportInstigator,TeleportDelay);


}

void AMyActionRPGDashProjectile::TeleportInstigator()
{
	AActor *ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(),ActorToTeleport->GetActorRotation(),false,false);
	}
	Destroy();
}

void AMyActionRPGDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_DelayDetonate,this,&ThisClass::Explode,DetonateDelay);
	if (AActor * MyInstigator = GetInstigator()){SphereComponent->IgnoreActorWhenMoving(MyInstigator,true);}
}

