
#include "AI/ARPG_BTService_CheckHealth.h"
#include "ActionRPGAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


UARPG_BTService_CheckHealth::UARPG_BTService_CheckHealth()
{
	LowHealthFraction = 0.9f;
}


void UARPG_BTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (Pawn)
	{
		UActionRPGAttributeComponent* AttributeComponent = UActionRPGAttributeComponent::GetAttributes(Pawn);
		if (ensure(AttributeComponent))
		{
			const bool bLowHealth = ((AttributeComponent->GetHealth()/AttributeComponent->GetMaxHealth()) < LowHealthFraction);
			
			
			if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
			{
				BlackboardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName,bLowHealth);
			}
		}
	}
}
