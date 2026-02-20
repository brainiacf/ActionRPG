#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActionRPGAICharacter.generated.h"

class UPawnSensingComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UActionRPGAttributeComponent;
class UUserWidget;
class UActionRPGWorldUserWidget;
class UActionRPGActionComponent;


UCLASS()
class ACTIONRPG_API AActionRPGAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AActionRPGAICharacter();

protected:
	
	UPROPERTY(EditAnywhere,Category=Components)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;
	
	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UActionRPGWorldUserWidget> ActiveHealthBar;
	
	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<UUserWidget> SpottedWidgetClass;
	
	/*UPROPERTY(Transient)
	TObjectPtr<UActionRPGWorldUserWidget> SpottedWidget;*/
	
	

	UPROPERTY(EditDefaultsOnly, Category=Action)
	TObjectPtr<UActionRPGActionComponent> ActionComponent;

	UPROPERTY(VisibleAnywhere, Category=Effects)
	FName TimeToHitParameterName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components, meta=(AllowPrivateAccess=true))
	TObjectPtr<UActionRPGAttributeComponent> AttributeComponent;

	//func.

	void SetTargetActor(AActor* NewTarget);


	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UActionRPGAttributeComponent* OwningComp, float NewHealth,
	                    float Delta);

	void IsAlive();

protected:
	virtual void PostInitializeComponents() override;
	
	/*when AI sees pawn it stores the value to the Blackboard key component we have the component, we have the component,we can return a cast from that Key */
	
	AActor* GetTargetActor();

};
