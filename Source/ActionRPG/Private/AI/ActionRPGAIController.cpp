
#include "AI/ActionRPGAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AActionRPGAIController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviorTree,TEXT(" bro without BT, AI will have No BT")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
