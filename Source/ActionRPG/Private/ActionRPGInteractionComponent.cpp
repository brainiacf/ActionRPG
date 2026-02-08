
#include "ActionRPGInteractionComponent.h"

#include "ActionRPGGameplayInterface.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Widgets/Text/STextScroller.h"
#include "ActionRPGWorldUserWidget.h"
static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"),
	false,
	TEXT("Enable debug lines for interact component"),
	ECVF_Cheat);
// Sets default values for this component's properties
UActionRPGInteractionComponent::UActionRPGInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	 CollisionChannel = ECC_WorldDynamic;
}

void UActionRPGInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FindBestInteractable();
}


void UActionRPGInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	//FCollisionObjectQueryParams -> filters which object types a trace should consider during collisions 
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	AActor* MyOwner = GetOwner();
	
	FVector EyesLocation;
	FRotator EyesRotation;
	
	MyOwner->GetActorEyesViewPoint(EyesLocation,EyesRotation);
	FVector End = EyesLocation +(EyesRotation.Vector()*TraceDistance);
	//float Radius = 30.0f;
	
	//Hit
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius );
	
	//SweepMultiByObjectType->performs a collision sweep that detects all the hits along a shape path filtered by object types.
	//FQuat::Identity -> represents no rotation in UE
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits,EyesLocation,End,FQuat::Identity,ObjectQueryParams,Shape);
	FColor LineColor = bBlockingHit ? FColor::Red : FColor::Blue;
	
	// Clear ref before trying to fill
	FocusedActor = nullptr;
	// for every Hit
	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(),Hit.ImpactPoint,TraceRadius,10,FColor::Green,false,2.0f);
		}
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UActionRPGGameplayInterface>())
			{
				FocusedActor = HitActor;	
				break;
			}
		}
	}
	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{ 
			DefaultWidgetInstance = CreateWidget<UActionRPGWorldUserWidget>(GetWorld(),DefaultWidgetClass);	
		}
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();	
			}
			
		}
	}
	else
	{
		if (DefaultWidgetInstance )
		{
			DefaultWidgetInstance->RemoveFromParent()	/* wow this widget got no parent now */;
		}
	}
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(),EyesLocation,End,LineColor,false,2.0f,0,1.0f);
	}
	
}


void UActionRPGInteractionComponent::PrimaryInteract()
{
	
	if (!FocusedActor)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Green,TEXT("Bro who you focusing at?? "));
		}
		return;
	}
	//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("HitActor"));
	APawn*MyPawn = Cast<APawn>(GetOwner());
	IActionRPGGameplayInterface::Execute_Interact(FocusedActor,MyPawn);
			
}
