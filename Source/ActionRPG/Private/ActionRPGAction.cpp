
#include "ActionRPGAction.h"
#include "Engine/World.h"
#include "ActionRPGActionComponent.h"




void UActionRPGAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Start %s"),*GetNameSafe(this));
	UActionRPGActionComponent*Comp = GetOwningComponent();
	
	// call ActiveGameplayTags.AppendTags
}

void UActionRPGAction::StartAction_Implementation(AActor* Instigator)
{
}

UWorld* UActionRPGAction::GetWorld() const
{
	return Super::GetWorld();
}
bool UActionRPGAction::CanStart_Implementation(AActor* Instigator)
{
	return true;
}

UActionRPGActionComponent* UActionRPGAction::GetOwningComponent()
{
	//The purpose of this function is to retrieve the specific Component that "owns" or created this Action.
	//it allows the action to talk to the component that runs it.
	
	return Cast<UActionRPGActionComponent>(GetOuter());
	
}


bool UActionRPGAction::IsRunning() const
{
	return true;
}



