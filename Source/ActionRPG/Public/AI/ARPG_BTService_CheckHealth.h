
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ARPG_BTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UARPG_BTService_CheckHealth : public UBTService
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,Category=AI)
	FBlackboardKeySelector LowHealthKey;
	
	UPROPERTY(EditAnywhere,Category=AI)
	float LowHealthFraction;
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UARPG_BTService_CheckHealth();
	
};
