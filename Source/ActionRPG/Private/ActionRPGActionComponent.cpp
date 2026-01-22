#include "ActionRPGActionComponent.h"

UActionRPGActionComponent::UActionRPGActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UActionRPGActionComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UActionRPGActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

