

#include "ActionRPGCharacter.h"
#include "ActionRPGActionComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include  "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "ActionRPGInteractionComponent.h"
#include "ActionRPGAttributeComponent.h"
#include "ActionRPGProjectileBase.h"
#include "Engine/Engine.h"
#include "Components/AudioComponent.h"
#include "ActionRPGActionComponent.h"
#include "Net/UnrealNetwork.h"

AActionRPGCharacter::AActionRPGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f,96.f);
	// dont rotate when camera Rotates 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	//Orient RotationToMovement -> Moves the Character to face the direction they are moving in 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,500.0f,0.0f);
	
	//init
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true; // rotate the arm based on the controller 
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false; // It determines whether that specific component should ignore its own local rotation and instead lock itself to the Player Controller’s rotation (aim direction).
	
	InteractionComponent = CreateDefaultSubobject<UActionRPGInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<UActionRPGAttributeComponent>("Attribute");
	ActionComponent = CreateDefaultSubobject<UActionRPGActionComponent>("ActionComponent");
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);
}

void AActionRPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 1. Get the Player Controller
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// 2. ONLY do this for the local player (The person sitting at the computer)
		if (IsLocallyControlled())
		{
			// 3. Add Mapping Context (The fix from before)
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}

			// 4. --- NEW FIX: Force Input Mode ---
			// Hide the mouse cursor so it locks to the center of the screen
			PC->SetShowMouseCursor(false); 

			// Tell Unreal: "Ignore UI, send all keyboard/mouse clicks to the Game"
			FInputModeGameOnly InputModeData;
			PC->SetInputMode(InputModeData);
		}
	}
	
}

void AActionRPGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChange.AddDynamic(this,&AActionRPGCharacter::OnHealthChanged);
	// Add input Mapping Context

}

void AActionRPGCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
}

void AActionRPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActionRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent called!"));

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,this,&ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&ACharacter::StopJumping);
		//
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AActionRPGCharacter::Move);
		//
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&AActionRPGCharacter::Look);
		//
		EnhancedInputComponent->BindAction(MainAttackAction,ETriggerEvent::Completed,this,&AActionRPGCharacter::PrimaryAttack);
		EnhancedInputComponent->BindAction(SecondaryAttack,ETriggerEvent::Completed,this,&AActionRPGCharacter::BlackholeAttack);
		//
		EnhancedInputComponent->BindAction(InteractAction,ETriggerEvent::Completed,this,&AActionRPGCharacter::PrimaryInteract);
		//
		EnhancedInputComponent->BindAction(DashAction,ETriggerEvent::Completed,this,&AActionRPGCharacter::Dash);
		//
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Started,this,&AActionRPGCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Completed,this,&AActionRPGCharacter::SprintStop);
	}
	else
	{
		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Yellow,TEXT("failed to find EIC. character input setup problem"));
		}
	}

}

void AActionRPGCharacter::HealSelf(float Amount)
{
	AttributeComponent->ApplyHealthChange(this,Amount);
}

void AActionRPGCharacter::Move(const FInputActionValue& Value)
{
	if (HasAuthority()) // Only print for the Server player
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, TEXT("SERVER IS RECEIVING INPUT!"));
	}
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator (0,Rotation.Yaw,0);
		//
		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//
		AddMovementInput(ForwardVector,MovementVector.Y);
		AddMovementInput(RightVector,MovementVector.X);
		
	}
}

void AActionRPGCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

}

void AActionRPGCharacter::SprintStart(const FInputActionValue& Value)
{
	ActionComponent->StartActionByName(this,"Sprint");
}

void AActionRPGCharacter::SprintStop(const FInputActionValue& Value)
{
	ActionComponent->StopActionByName(this,"Sprint");
}

void AActionRPGCharacter::PrimaryAttack()
{
	//if (GEngine){GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("Fire"));}
	ActionComponent->StartActionByName(this,"PrimaryAttack");
	
	
}

void AActionRPGCharacter::BlackholeAttack()
{
	// black hole attack can only be started if you have rage > 20, it will cost 10 
	/*Attribute Component -> get Rage -> Check Rage if Greater than 20 then do Start ActionByName, and subtract 10 from rage */
	if (AttributeComponent->GetRage() > 20.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Yellow,TEXT("Rage is 20 >"));
		ActionComponent->StartActionByName(this,"BlackholeAttack");
		AttributeComponent->ApplyRageChange(this,-10);

	}
	
}

void AActionRPGCharacter::Dash()
{
	ActionComponent->StartActionByName(this,"Dash");
}

void AActionRPGCharacter::PrimaryInteract()
{
}

void AActionRPGCharacter::OnHealthChanged(AActor* InstigatorActor, UActionRPGAttributeComponent* OwningComp,
	float NewHealth, float Delta)
{
	if (Delta<0.0f)
	{
		UE_LOG(LogTemp,Log,TEXT("Delta is less than 0 in OnHealthChanged ActionCharacter"));
	}
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController*PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

FVector AActionRPGCharacter::GetPawnViewLocation() const
{
	return FollowCamera->GetComponentLocation();
}

