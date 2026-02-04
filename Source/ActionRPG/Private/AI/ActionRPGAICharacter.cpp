#include "AI/ActionRPGAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "DrawDebugHelpers.h"
#include "ActionRPGAttributeComponent.h"
#include "BrainComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActionRPGWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "ActionRPGActionComponent.h"
#include "Perception/PawnSensingComponent.h"

AActionRPGAICharacter::AActionRPGAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	PawnSensingComponent= CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	
	
	
	//At
	AttributeComponent = CreateDefaultSubobject<UActionRPGAttributeComponent>("AttributeComponent");
	TimeToHitParameterName = "TimeToHit";
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	ActionComponent = CreateDefaultSubobject<UActionRPGActionComponent>("ActionComponent");
}

void AActionRPGAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this,&ThisClass::OnPawnSeen);
	
	AttributeComponent->OnHealthChange.AddDynamic(this,&ThisClass::OnHealthChange);
}


void AActionRPGAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(),GetActorLocation(),"PlayerSpotted",nullptr,FColor::Green,4.0f,true);
}

void AActionRPGAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor",NewTarget);
		//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Black,(TEXT("AIC is false mkc")));
	}
}



void AActionRPGAICharacter::OnHealthChange(AActor* InstigatorActor, UActionRPGAttributeComponent* OwningComp,
                                           float NewHealth, float Delta)
{
	if (Delta < 0)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UActionRPGWorldUserWidget>(GetWorld(),HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachActor = this;
				UActionRPGWorldUserWidget::AddToRootCanvasPanel(ActiveHealthBar);
			
				
			}
		}
	}
	GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParameterName,GetWorld()->TimeSeconds);
	if (NewHealth <= 0.0f)
	{
		//stops BT, No BT for AI pawn since it died RIP-> there will be no BT after you die :)
		AAIController* AIC = Cast<AAIController>(GetController());
		if (AIC)
		{
			//this is the moral of life/ life will be better after you die, till then suffer // logic is cause of all BT
			AIC->GetBrainComponent()->StopLogic("Killed");
		}
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//wow ragdoll 
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();
		
		//Set lifespan
		SetLifeSpan(10.0f);
		
	}
}

void AActionRPGAICharacter::IsAlive()
{
}
