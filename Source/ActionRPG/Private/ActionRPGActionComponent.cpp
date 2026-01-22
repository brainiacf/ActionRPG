#include "ActionRPGActionComponent.h"

UActionRPGActionComponent::UActionRPGActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionRPGActionComponent::AddAction(TSubclassOf<UActionRPGAction> ActionClass)
{
}

bool UActionRPGActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	return true;
}

bool UActionRPGActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	return true;
}


void UActionRPGActionComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UActionRPGActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

