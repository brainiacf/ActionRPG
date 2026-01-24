
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ActionRPGAction.generated.h"

class UWorld;
class UActionRPGActionComponent;
/**
 * UCLASS(Blueprintable) -> allows us to Create Blueprint subclasses (eg-> SprintAction)
 */
UCLASS(Blueprintable)
class ACTIONRPG_API UActionRPGAction : public UObject
{
	GENERATED_BODY()
protected:
	// helper func to easily access the component that "owns" this action 
	UFUNCTION( BlueprintCallable ,Category=Action)
	UActionRPGActionComponent*GetOwningComponent();
	
	/*tags added to owning actor when action is activated, removed when action is stopped / completed.
	* tags applied to the owning actor while the action is running */
	UPROPERTY(EditDefaultsOnly,Category=Tags)
	FGameplayTagContainer GrantTags;
	
	/*Action can only be start if owning actor has none of these tags applied
	 * If the owner has any of these tags the action cannot start 
	 */
	UPROPERTY(EditDefaultsOnly,Category=Tags)
	FGameplayTagContainer BlockedTags;
	/*Internal flag to track if we are currently active */
	bool bIsRunning;
public:
	// Unique ID for this action (e.g. Sprint,attack ) used to trigger the it
	UPROPERTY(EditDefaultsOnly,Category=Action)
	FName ActionName;
	
	//Native Event can be implemented in C++ or in BP//
	
	// Logic for what happens when the action actually begins.
	UFUNCTION( BlueprintNativeEvent,Category=Action )
	void StartAction(AActor*Instigator);
	
	//Logic for cleaning up(removing effects, reseting vars ) when action ends;
	UFUNCTION( BlueprintNativeEvent,Category=Action )
	void StopAction(AActor*Instigator);
	
	//Custom override needed because UObject don't inherently know about the world
	UWorld*GetWorld() const override;
	
	// checks Blocked tags and bIsRunning to see if we are allowed to execute 
	UFUNCTION(BlueprintNativeEvent,Category=Action)
	bool CanStart(AActor*Instigator);
	
	// getter for bIsRunning;
	UFUNCTION(BlueprintCallable,Category=Action)
	bool IsRunning() const;


};
