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
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Tags)
	FGameplayTagContainer ActiveGameplayTags;
	
	UFUNCTION(BlueprintCallable,Category=Actions)
	void AddAction(TSubclassOf<UActionRPGAction> ActionClass);
	
	UFUNCTION(BlueprintCallable,Category=Actions)
	bool StartActionByName(AActor*Instigator,FName ActionName);
	
	UFUNCTION(BlueprintCallable,Category=Actions)
	bool StopActionByName(AActor*Instigator,FName ActionName);
	

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TArray<UActionRPGAction*> Actions;
	
	UPROPERTY(EditAnywhere,Category=Actions)
	TArray<TSubclassOf<UActionRPGAction>> DefaultActions;
	

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
