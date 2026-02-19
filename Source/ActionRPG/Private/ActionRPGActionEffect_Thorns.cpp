#include "ActionRPGActionEffect_Thorns.h"
#include "ActionRPGAttributeComponent.h"
#include "ActionRPGActionComponent.h"
#include "ActionRPGGameplayFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ActionRPGActionEffect_Thorns)/*macro -> tells UBT to include auto gen, reflection boilerplate code directly here to improve compile time lets see what happens now */

UActionRPGActionEffect_Thorns::UActionRPGActionEffect_Thorns()
{
	ReflectFraction = 0.2f;
	
	Duration = 0.0f;
	Period = 0.0f;
	
}

void UActionRPGActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	// Start Listening 
	UActionRPGAttributeComponent * Attributes = UActionRPGAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChange.AddDynamic(this,&UActionRPGActionEffect_Thorns::OnHealthChange);
	}
}

void UActionRPGActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	// Stop Listening 
	UActionRPGAttributeComponent * Attributes = UActionRPGAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChange.RemoveDynamic(this,&UActionRPGActionEffect_Thorns::OnHealthChange);
	}
}


void UActionRPGActionEffect_Thorns::OnHealthChange(AActor* InstigatorActor, UActionRPGAttributeComponent* OwningComp,
                                                   float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();
	//DO
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// round to the nearest to avoid ugly damage numbers and tine reflections 
		int32 ReflectedAmount = FMath::RoundToInt((Delta*ReflectFraction));
		if (ReflectedAmount == 0)
		{
			return;
		}
		//flip to +ve so that we don't end up healing ourselves when passes into damage 
		ReflectedAmount = FMath::Abs(ReflectedAmount);
		
		//return Damage sende... 
		UActionRPGGameplayFunctionLibrary::ApplyDamage(OwningActor,InstigatorActor,ReflectedAmount);
	}
	
}
