
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ActionRPGPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged,AActionRPGPlayerState*, PlayerState,int32,NewCredits,int32,Delta);

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AActionRPGPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly,Category = "Credits")
	int32 Credits;
	
public:
	UFUNCTION(BlueprintCallable,Category="Credits")
	int32 GetCredits() const;
	
	UFUNCTION(BlueprintCallable,Category="Credits")
	void AddCredits(int32 Delta);
	
	UFUNCTION(BlueprintCallable,Category="Credits")
	bool RemoveCredits(int32 Delta);
	
	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnCreditsChanged OnCreditsChanged;
	
	
};
