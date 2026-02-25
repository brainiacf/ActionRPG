
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ActionRPGPlayerController.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged,APawn*,NewPawn);
/**
 * 
 */
UCLASS()
class ACTIONRPG_API AActionRPGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;
	
	virtual void SetPawn(APawn* InPawn) override;
	
	
};
