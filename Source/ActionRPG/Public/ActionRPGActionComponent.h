#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "ActionRPGActionComponent.generated.h"

/*the main goal of this component is to keep a list of actions*/

class UActionRPGAction;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONRPG_API UActionRPGActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActionRPGActionComponent();
	//the container holds all currently active tags on this character. // e.g. status, stunned, Status.Sprinting
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Tags)
	FGameplayTagContainer ActiveGameplayTags;
	
	//Creates a new Action Instance and add it to the list. 
	UFUNCTION(BlueprintCallable,Category=Actions)
	void AddAction(TSubclassOf<UActionRPGAction> ActionClass);
	
	// Finds an action by 'ActionName' and tries to start it 
	UFUNCTION(BlueprintCallable,Category=Actions)
	bool StartActionByName(AActor*Instigator,FName ActionName);
	
	// Finds an action by 'ActionName' and tries to stop it 
	UFUNCTION(BlueprintCallable,Category=Actions)
	bool StopActionByName(AActor*Instigator,FName ActionName);
	

protected:
	virtual void BeginPlay() override;
	
	// the list of instantiated Action Object.
	UPROPERTY()
	TArray<UActionRPGAction*> Actions;
	
	// These get converted into real objects in BeginPlay
	UPROPERTY(EditAnywhere,Category=Actions)
	TArray<TSubclassOf<UActionRPGAction>> DefaultActions;
	

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
