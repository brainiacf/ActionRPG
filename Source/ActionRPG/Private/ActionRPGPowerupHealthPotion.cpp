
#include "ActionRPGPowerupHealthPotion.h"
#include "Components/MeshComponent.h"
#include "ActionRPGAttributeComponent.h"
#include "GameFramework/Pawn.h"
#include "ActionRPGPlayerState.h"


AActionRPGPowerupHealthPotion::AActionRPGPowerupHealthPotion()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);
	CreditCost = 50;
}

void AActionRPGPowerupHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	UActionRPGAttributeComponent* AttributeComponent = Cast<UActionRPGAttributeComponent>(InstigatorPawn->GetComponentByClass(UActionRPGAttributeComponent::StaticClass()));
	// check if Not already at max health 
	
	if (ensure(AttributeComponent)&& !AttributeComponent->IsFullHealth())
	{
		if (AActionRPGPlayerState* PS = InstigatorPawn->GetPlayerState<AActionRPGPlayerState>())
		{
			if (PS->RemoveCredits(CreditCost) && AttributeComponent->ApplyHealthChange(this,AttributeComponent->GetMaxHealth()))
			{
				// OnlyActivateif healed 
				HideAndCoolDownPowerUp(); 
			}
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"PowerUp Actor Created");

}
