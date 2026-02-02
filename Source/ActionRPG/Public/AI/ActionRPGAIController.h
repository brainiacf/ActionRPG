
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ActionRPGAIController.generated.h"
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class ACTIONRPG_API AActionRPGAIController : public AAIController
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,Category=AI)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
public:
	virtual void BeginPlay() override;
	
};
