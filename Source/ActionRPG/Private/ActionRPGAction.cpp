
#include "ActionRPGAction.h"
#include "Engine/World.h"
#include "ActionRPGActionComponent.h"
#include "ActionRPG/ActionRPG.h"
#include "Net/UnrealNetwork.h"

#include  UE_INLINE_GENERATED_CPP_BY_NAME(ActionRPGAction)

void UActionRPGAction::Initialize(UActionRPGActionComponent* NewActionComp)
{
	ActionComponent = NewActionComp;
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
void UActionRPGAction::StartAction_Implementation(AActor* Instigator)
{
	
	LogOnScreen(this,FString::Printf(TEXT("Started %s"), *ActionName.ToString()),FColor::Green);
	UActionRPGActionComponent*Comp = GetOwningComponent();
	
	//Add Our "GrantTags" to the component.
	// this marks the character with whatever status this aciton provides/
	
	Comp->ActiveGameplayTags.AppendTags(GrantTags);
	
	
	// mark as running so we don't start it twice.
	bIsRunning = true;
}

void UActionRPGAction::StopAction_Implementation(AActor* Instigator)
{
	
	LogOnScreen(this,FString::Printf(TEXT("Stopped %s"), *ActionName.ToString()),FColor::Green);
	UE_LOG(LogTemp, Log, TEXT(" Stop %s"), *GetNameSafe(this));
	
	//safely check: we shouldn't be stopping if we aren't running.
	//ensureAlways(bIsRunning);
	//get comp  
	UActionRPGActionComponent* Comp = GetOwningComponent();
	
	// Remove the tags we granted. the character is no longer in this "State
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);
	// mark it stopped
	bIsRunning = false;
	
}


UActionRPGActionComponent* UActionRPGAction::GetOwningComponent() const
{
	
	
	return ActionComponent;
}

void UActionRPGAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

void UActionRPGAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UActionRPGAction,bIsRunning);
	DOREPLIFETIME(UActionRPGAction,ActionComponent);
}


UWorld* UActionRPGAction::GetWorld() const
{
	
	// CRITICAL: UObjects don't know what "World" they are in by default.
	// We assume the "Outer" (the object that created this action) is the Component.
	// We cast the Outer to an Actor Component to Ask for IT for the world;
	AActor* Actor = Cast<AActor>(GetOuter());
	if(Actor){return Actor->GetWorld();}
	return nullptr;
}




bool UActionRPGAction::IsRunning() const
{
	return bIsRunning;
}



