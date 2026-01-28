
#include "AI/ARPG_BTService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARPG_BTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// check the dist between AI pawn and Target actor 
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComponent))
	{
		AActor *TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController *MyController = Cast<AAIController>(OwnerComp.GetAIOwner());
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					float Distance = FVector::Distance(TargetActor->GetActorLocation(),AIPawn->GetActorLocation());
					bool bWithinRange = Distance<2000.0f;
					bool bHasLOS = false ;
					if (bWithinRange)
					{
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}
					BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName,bWithinRange&&bHasLOS);
				}
			}
		}
	}
}
