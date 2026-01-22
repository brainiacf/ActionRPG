
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActionRPGAICharacter.generated.h"

UCLASS()
class ACTIONRPG_API AActionRPGAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AActionRPGAICharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
