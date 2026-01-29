
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GameFramework/Pawn.h"
#include "ActionRPGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

class UActionRPGInteractionComponent;
class UActionRPGActionComponent;
class UActionRPGAttributeComponent;

class UAnimMontage;
class UAudioComponent;

//
struct FInputActionValue;



UCLASS()
class ACTIONRPG_API AActionRPGCharacter : public ACharacter
{
	GENERATED_BODY()
	
// 
protected:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera)
	TObjectPtr<USpringArmComponent>CameraBoom;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera)
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= Components)
	TObjectPtr<UActionRPGInteractionComponent> InteractionComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category= Components)
	TObjectPtr<UActionRPGActionComponent> ActionComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = Components)
	TObjectPtr<UActionRPGAttributeComponent> AttributeComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Input)
	TObjectPtr<UAudioComponent> AudioComponent;
	
	
	// input
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Input)
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Input)
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Input)
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Input)
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Input)
	TObjectPtr<UInputAction> MainAttackAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Input)
	TObjectPtr<UInputAction> SecondaryAttack;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Input)
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Input)
	TObjectPtr<UInputAction> DashAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Input)
	TObjectPtr<UInputAction> SprintAction;
	
	UPROPERTY(EditAnywhere,Category= Weapon)
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere,Category= Weapon)
	TSubclassOf<AActor> BlackholeClass;
	
	UPROPERTY(EditAnywhere,Category= Weapon)
	TSubclassOf<AActor> DashProjectileClass;
	
	// Move funC
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	//sprint funC
	void SprintStart(const FInputActionValue& Value);
	void SprintStop(const FInputActionValue& Value);
	//attack funC
	UFUNCTION()
	void PrimaryAttack();
	UFUNCTION()
	void BlackholeAttack();
	//dash funC
	UFUNCTION()
	void Dash();
	//Interact funC
	UFUNCTION()
	void PrimaryInteract();
	// Health change 
	UFUNCTION()
	void OnHealthChanged(AActor*InstigatorActor,UActionRPGAttributeComponent*OwningComp,float NewHealth,float Delta);
	
	virtual FVector GetPawnViewLocation() const override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	AActionRPGCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void NotifyControllerChanged() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

};
