
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "EnvironmentQuery/EnvQueryTypes.h"                  // defines EEnvQueryStatus
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h" // if you use the wrapper in the signature

#include "ActionRPGGameModeBase.generated.h"

class UEnvQuery;
class UCurveFloat;
class AController;

UCLASS()
class ACTIONRPG_API AActionRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	AActionRPGGameModeBase();
	
protected:
	
	FTimerHandle TimerHandle_SpawnBots;
	
	UPROPERTY(EditDefaultsOnly,Category=AI)
	TObjectPtr<UEnvQuery> SpawnBotsQuery;
	
	UPROPERTY(EditDefaultsOnly,Category=AI)
	TObjectPtr<UCurveFloat> DifficultyCurve;
	
	UPROPERTY(EditDefaultsOnly,Category=AI)
	TSubclassOf<AActor> MinionClass;
	
	UFUNCTION()
	void SpawnBotTimerElapsed();
	
	UPROPERTY(EditAnywhere,Category=AI)
	float SpawnTimerInterval;
	
	UFUNCTION()//UEnvQueryInstanceBlueprintWrapper is the bridge that allows Blueprints to handle the results of an Environment Query System (EQS) test.
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,EEnvQueryStatus::Type QueryStatus);
	
	// GM does not do begin play cause it is responsible. of calling BP on all the classes.
public:
	
	virtual void StartPlay() override;
	
	virtual void OnActorKilled(AActor*VictimActor,AActor*Killer);
	
	UFUNCTION()
	void RespawnPlayerElapsed(AController*Controller);
	
	UFUNCTION(Exec)
	void KillAI();
	
};
