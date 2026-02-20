
#include "ActionRPGPowerupActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ActionRPGPowerupActor)/* ill be the best mf in game dev there will ever be. */

AActionRPGPowerupActor::AActionRPGPowerupActor()
{
	//PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComponent;
	RespawnTime = 10.0f;
	bIsActive = true;
	
	SetReplicates(true);
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
	bIsActive = true;
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
	if (HasAuthority())
	{
		bIsActive = false;	
		HideAndCoolDownPowerUp();
		Execute_Interact(this,CastChecked<APawn>(OtherActor));/*ill be 100% sure of the collision and then remove this cast checked, could be risky if anything other than Pawn comes around*/
	}
	
	
}
void AActionRPGPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AActionRPGPowerupActor, bIsActive);
}
void AActionRPGPowerupActor::OnRep_IsActive(bool bOldIsActive)
{
		SetPowerupState(bIsActive);
}
