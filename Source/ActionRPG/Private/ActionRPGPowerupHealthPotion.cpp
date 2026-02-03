
#include "ActionRPGPowerupHealthPotion.h"
#include "Components/MeshComponent.h"
#include "ActionRPGAttributeComponent.h"
#include "GameFramework/Pawn.h"


AActionRPGPowerupHealthPotion::AActionRPGPowerupHealthPotion()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);
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
		if (AttributeComponent->ApplyHealthChange(this,AttributeComponent->GetMaxHealth()))
		{
			HideAndCoolDownPowerUp();
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"PowerUp Actor Created");

}
