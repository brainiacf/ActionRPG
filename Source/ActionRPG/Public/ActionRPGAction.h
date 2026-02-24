
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ActionRPGAction.generated.h"

class UWorld;
class UActionRPGActionComponent;
class UTexture2D;


USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	bool bIsRunning;
	
	UPROPERTY()
	AActor* Instigator;
	
};


UCLASS(Blueprintable)
class ACTIONRPG_API UActionRPGAction : public UObject
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(Replicated)
	UActionRPGActionComponent* ActionComponent ;
	
	//TSoftObjectPtr -> forces the asset to load into memory immediately when object loads, stores the path to the asset// Used to prevent massive load times and memory usage 
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=UI)
	TSoftObjectPtr<UTexture2D> Icon;
	
	/*//Transient-> tells UE to not save this variable to disk 
	UPROPERTY(BlueprintReadWrite,Transient,Category=Action)
	TObjectPtr<UActionRPGActionComponent> ActionComp;*/
	
	// helper func to easily access the component that "owns" this action 
	UFUNCTION( BlueprintCallable ,Category=Action)
	UActionRPGActionComponent*GetOwningComponent() const;
	
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
	UPROPERTY(ReplicatedUsing="OnRep_IsRunning")
	FActionRepData RepData;
	//bool bIsRunning;
	
	UFUNCTION()
	void OnRep_IsRunning();
	
	
	/*Action NickName to Start/Stop without a reference to the object, ill implement this later */
	UPROPERTY(EditDefaultsOnly,Category=Action)
	FGameplayTag ActivationTag;
	
public:
	
	/*auto Initialize(UActionRPGActionComponent* NewActionComp) -> void;*/
	
	UFUNCTION(BlueprintPure)
	FGameplayTag GetActivationTag()const
	{
		return ActivationTag;
	}
	UFUNCTION(BlueprintPure)
	bool IsAutoStart() const
	{
		return bAutoStart;
	}
	void Initialize(UActionRPGActionComponent* NewActionComp);
	
	// start immediately when added to an action component, Action Component checks while adding the action  if it can auto start -> then starts the action
	UPROPERTY(EditDefaultsOnly,Category=Action)
	bool bAutoStart;
	
	// Unique ID for this action (e.g. Sprint,attack ) used to trigger it
	UPROPERTY(EditDefaultsOnly,Category=Action)
	FName ActionName;
	
	//Native Event can be implemented in C++ or in BP//
	
	// Logic for what happens when the action actually begins.
	UFUNCTION( BlueprintNativeEvent,Category=Action )
	void StartAction(AActor*Instigator);
	
	//Logic for cleaning up(removing effects, reseting vars ) when action ends;
	UFUNCTION( BlueprintCallable,BlueprintNativeEvent,Category=Action )
	void StopAction(AActor*Instigator);
	
	//Custom override needed because UObject don't inherently know about the world
	UWorld*GetWorld() const override;
	
	// checks Blocked tags and bIsRunning to see if we are allowed to execute 
	UFUNCTION(BlueprintNativeEvent,Category=Action)
	bool CanStart(AActor*Instigator);
	
	// getter for bIsRunning;
	UFUNCTION(BlueprintCallable,Category=Action)
	bool IsRunning() const;
	
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}


};
