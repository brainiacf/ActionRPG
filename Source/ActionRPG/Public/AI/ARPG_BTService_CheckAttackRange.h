
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ARPG_BTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARPG_BTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere,Category=AI)
	FBlackboardKeySelector AttackRangeKey;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
