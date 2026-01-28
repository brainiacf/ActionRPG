
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActionRPGAICharacter.generated.h"

class UPawnSensingComponent;
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
	UPROPERTY(VisibleAnywhere,Category=Components)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;
	
	UPROPERTY(EditDefaultsOnly,Category=UI)
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(EditDefaultsOnly,Category=UI)
	TObjectPtr<UActionRPGWorldUserWidget> ActiveHealthBar;
	
	UPROPERTY(EditDefaultsOnly,Category=Action)
	TObjectPtr<UActionRPGActionComponent> ActionComponent;
	
	UPROPERTY(VisibleAnywhere,Category=Effects)
	FName TimeToHitParameterName;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Components,meta=(AllowPrivateAccess=true))
	TObjectPtr<UActionRPGAttributeComponent> AttributeComponent;
	
	//func.
	
	void SetTargetActor(AActor* NewTarget);
	
	UFUNCTION()
	void OnPawnSeen(APawn *Pawn);
	
	UFUNCTION()
	void OnHealthChange(AActor *InstigatorActor, UActionRPGAttributeComponent *OwningComp, float NewHealth, float Delta);
	
	void IsAlive();
	
	
	
	
	
protected:
	virtual void PostInitializeComponents() override;



};
