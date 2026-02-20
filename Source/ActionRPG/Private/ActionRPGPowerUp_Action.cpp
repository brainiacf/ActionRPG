
#include "ActionRPGPowerUp_Action.h"
#include "ActionRPGAction.h"
#include "ActionRPGActionComponent.h"

void AActionRPGPowerUp_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}
	UActionRPGActionComponent* ActionComponent = Cast<UActionRPGActionComponent>(InstigatorPawn->GetComponentByClass(UActionRPGActionComponent::StaticClass()));
	if (ActionComponent)
	{
		if (ActionComponent->GetAction(ActionToGrant))
		{
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known"),*GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,DebugMsg);
			return;
		}
		ActionComponent->AddAction(InstigatorPawn,ActionToGrant);
		HideAndCoolDownPowerUp();
	}
}
