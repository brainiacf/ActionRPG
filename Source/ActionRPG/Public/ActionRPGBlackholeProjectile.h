// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRPGProjectileBase.h"
#include "ActionRPGBlackholeProjectile.generated.h"

class URadialForceComponent;

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AActionRPGBlackholeProjectile : public AActionRPGProjectileBase
{
	GENERATED_BODY()
	AActionRPGBlackholeProjectile();
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<URadialForceComponent> RadialForceComponent;
	
};
