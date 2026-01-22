
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ActionRPGAction.generated.h"

class UWorld;
class UActionRPGActionComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONRPG_API UActionRPGAction : public UObject
{
	GENERATED_BODY()
protected:
//	UFUNCTION( BlueprintCallable ,Category=Action)
//	UActionRPGActionComponent*GetOwningComponent();
	
	/*tags added to owning actor when action is activated, removed when action is stopped / completed. */
	UPROPERTY(EditDefaultsOnly,Category=Tags)
	FGameplayTagContainer GrantTags;
	
	/*Action can only be start if owning actor has none of these tags applied*/
	UPROPERTY(EditDefaultsOnly,Category=Tags)
	FGameplayTagContainer BlockedTags;
	
	bool bIsRunning;
public:
	UPROPERTY(EditDefaultsOnly,Category=Action)
	FName ActionName;
	
	//UFUNCTION( BlueprintCallable )
	
};
