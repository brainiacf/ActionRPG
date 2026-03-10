// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActionRPGMonsterData.generated.h"

class UActionRPGAction;

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UActionRPGMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="SpawnInfo")
	TSubclassOf<AActor> MonsterClass;
	
	/*Actions/ buffs to grant this Monster*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="SpawnInfo")
	TArray<TSubclassOf<UActionRPGAction>> Actions;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="UI")
	TObjectPtr<UTexture2D> Icon;

};

