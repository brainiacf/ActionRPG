
#include "ActionRPGPowerupActor.h"

#include "Components/SphereComponent.h"

AActionRPGPowerupActor::AActionRPGPowerupActor()
{
	//PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComponent;
	RespawnTime = 10.0f;
	

}
void AActionRPGPowerupActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnSphereOverlap);
}


void AActionRPGPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//IActionRPGGameplayInterface::Interact_Implementation(InstigatorPawn);
}

void AActionRPGPowerupActor::ShowPowerUp()
{
	SetPowerupState(true);
}

void AActionRPGPowerupActor::HideAndCoolDownPowerUp()
{
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer,this,&ThisClass::ShowPowerUp,RespawnTime);
}

void AActionRPGPowerupActor::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	RootComponent->SetVisibility(bNewIsActive,true);
}
void AActionRPGPowerupActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Execute_Interact(this,CastChecked<APawn>(OtherActor));
}