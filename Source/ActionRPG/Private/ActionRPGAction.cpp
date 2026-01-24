
#include "ActionRPGAction.h"
#include "Engine/World.h"
#include "ActionRPGActionComponent.h"




void UActionRPGAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT(" Stop %s"), *GetNameSafe(this));
	
	//safely check: we shouldn't be stopping if we aren't running.
	ensureAlways(bIsRunning);
	//get comp  
	UActionRPGActionComponent* Comp = GetOwningComponent();
	
	// Remove the tags we granted. the character is no longer in this "State
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);
	// mark it stopped
	bIsRunning = false;
	
}

void UActionRPGAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Start %s"),*GetNameSafe(this));
	// get the component that owns us 
	UActionRPGActionComponent*Comp = GetOwningComponent();
	
	//Add Our "GrantTags" to the component.
	// this marks the character with whatever status this aciton provides/
	
	Comp->ActiveGameplayTags.AppendTags(GrantTags);
	
	// mark as running so we don't start it twice.
	bIsRunning = true;
}

UWorld* UActionRPGAction::GetWorld() const
{
	
	// CRITICAL: UObjects don't know what "World" they are in by default.
	// We assume the "Outer" (the object that created this action) is the Component.
	// We cast the Outer to an Actor Component to Ask for IT for the world;
	UActorComponent*Comp = Cast<UActorComponent>(GetOuter());
	if(Comp){return Comp->GetWorld();}
	return nullptr;
}
bool UActionRPGAction::CanStart_Implementation(AActor* Instigator)
{
	// Rule 1 -> Cannot Start if already running.
	if (IsRunning())
	{return false;}
	
	//Rule 2 -> Check for blocked tags 
	UActionRPGActionComponent*Comp = GetOwningComponent();
	
	// "HasAny" checks if the component has ANY of the tags in out Blocked list; 
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags)){return false;}
	
	return true;

}



bool UActionRPGAction::IsRunning() const
{
	return bIsRunning;
}

UActionRPGActionComponent* UActionRPGAction::GetOwningComponent()
{
	//The purpose of this function is to retrieve the specific Component that "owns" or created this Action.
	//it allows the action to talk to the component that runs it.
	// The "Outer" is set when we spawn this object in AActionComponent::AddAction
	return Cast<UActionRPGActionComponent>(GetOuter());
	
}



