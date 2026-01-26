#include "ActionRPGGameplayFunctionLibrary.h"
#include "ActionRPGAttributeComponent.h"



bool UActionRPGGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UActionRPGAttributeComponent *AttributeComponent = UActionRPGAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(DamageCauser,-DamageAmount);
	}
	return false;
}

bool UActionRPGGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor,
                                                               float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser,TargetActor,DamageAmount))
	{
		UPrimitiveComponent*HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.MyBoneName))
		{
			//direction is target-origin
			FVector Direction = HitResult.TraceEnd-HitResult.TraceStart;
			Direction.Normalize();
			HitComp->AddImpulseAtLocation(Direction*300000.0f,HitResult.ImpactNormal,HitResult.MyBoneName);
		}
		return true;
	
	}
	return false;
}
