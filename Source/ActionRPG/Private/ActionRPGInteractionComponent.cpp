
#include "ActionRPGInteractionComponent.h"

#include "ActionRPGGameplayInterface.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Widgets/Text/STextScroller.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"),
	false,
	TEXT("Enable debug lines for interact component"),
	ECVF_Cheat);
// Sets default values for this component's properties
UActionRPGInteractionComponent::UActionRPGInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UActionRPGInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	//FCollisionObjectQueryParams -> filters which object types a trace should consider during collisions 
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	AActor* MyOwner = GetOwner();
	
	FVector EyesLocation;
	FRotator EyesRotation;
	
	MyOwner->GetActorEyesViewPoint(EyesLocation,EyesRotation);
	FVector End = EyesLocation +(EyesRotation.Vector()*300);
	float Radius = 30.0f;
	
	//Hit
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	
	//SweepMultiByObjectType->performs a collision sweep that detects all the hits along a shape path filtered by object types.
	//FQuat::Identity -> represents no rotation in UE
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits,EyesLocation,End,FQuat::Identity,ObjectQueryParams,Shape);
	FColor LineColor = bBlockingHit ? FColor::Red : FColor::Blue;
	
	// for every Hit 
	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(),Hit.ImpactPoint,Radius,10,FColor::Green,false,2.0f);
		}
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UActionRPGGameplayInterface>())
			{
				GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("HitActor"));
				APawn*MyPawn = Cast<APawn>(MyOwner);
				IActionRPGGameplayInterface::Execute_Interact(HitActor,MyPawn);
				break;
			}
		}
	}
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(),EyesLocation,End,LineColor,false,2.0f,0,1.0f);
	}
	
}

