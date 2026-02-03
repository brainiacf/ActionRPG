#include "ActionRPGActionComponent.h"
#include "ActionRPGAction.h"

UActionRPGActionComponent::UActionRPGActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UActionRPGActionComponent::BeginPlay()
{
	Super::BeginPlay();
	// OnStartup loop through the classes selected in the editor (DefaultActions) and turn them into real objects. 
	
	for (TSubclassOf<UActionRPGAction> ActionClass :DefaultActions)
	{
		AddAction(ActionClass);
	}

}
void UActionRPGActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FString DebugMsg = GetNameSafe(GetOwner()) + ":" +ActiveGameplayTags.ToStringSimple();
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Green,DebugMsg);
	}

}

void UActionRPGActionComponent::AddAction(TSubclassOf<UActionRPGAction> ActionClass)
{
	if (!ensure(ActionClass)){return;}
	
	// NewObject Creates the Instance.
	// param 'this' Sets the component as the 'outer' of the new Action.
	// This allows the action to call GetOuter() to find this component later. 
	
	UActionRPGAction* NewAction = NewObject<UActionRPGAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UActionRPGActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	UE_LOG(LogTemp,Log,TEXT("Starting new Action"));
	// loop through all instantiated actions to find the one we want.
	for (UActionRPGAction*Action : Actions)
	{
		if ( Action && Action->ActionName == ActionName)
		{
			// we found the action. check if it is allowed to run 
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("failed to run: %s"),*ActionName.ToString());
				//GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Red,FailedMsg);
				continue;
			}
			// it's allowed! Run the Logic
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UActionRPGActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping mewo Action "));
	for (UActionRPGAction*Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			// can only stop it if it is running 
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}







