
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARPG_BTTask_RangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARPG_BTTask_RangeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere,Category = "AI")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere,Category = "AI")
	float MaxBulletSpead;
	
public:
	UARPG_BTTask_RangeAttack();
};
