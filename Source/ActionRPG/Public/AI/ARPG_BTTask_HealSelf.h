
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARPG_BTTask_HealSelf.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARPG_BTTask_HealSelf : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
