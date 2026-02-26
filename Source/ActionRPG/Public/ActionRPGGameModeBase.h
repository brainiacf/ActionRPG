
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "EnvironmentQuery/EnvQueryTypes.h"                  // defines EEnvQueryStatus
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h" // if you use the wrapper in the signature

#include "ActionRPGGameModeBase.generated.h"

class UEnvQuery;
class UCurveFloat;
class AController;
class UActionRPGSaveGame;

UCLASS()
class ACTIONRPG_API AActionRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	AActionRPGGameModeBase();
	
protected:
	//save
	FString SlotName;
	UPROPERTY()
	TObjectPtr<UActionRPGSaveGame>  CurrentSaveGame;
	//credits
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category=AI)
	int32 InitialSpawnCredits;
	UPROPERTY(EditDefaultsOnly,Category=Credits)
	int32 CreditsPerKill;
	
	FTimerHandle TimerHandle_SpawnBots;
	
	UPROPERTY(EditDefaultsOnly,Category=AI)
	TObjectPtr<UEnvQuery> SpawnBotsQuery;
	
	UPROPERTY(EditDefaultsOnly,Category=AI)
	TObjectPtr<UCurveFloat> DifficultyCurve;
	
	UPROPERTY(EditDefaultsOnly,Category=AI)
	TSubclassOf<AActor> MinionClass;
	
	//PowerUps
	UPROPERTY(EditDefaultsOnly,Category=PowerUps)
	TObjectPtr<UEnvQuery> PowerUpSpawnQuery;
	
	UPROPERTY(EditDefaultsOnly,Category=PowerUps)
	TArray<TSubclassOf<AActor>> PowerUpClasses;
	
	UPROPERTY(EditDefaultsOnly,Category=PowerUp)
	float RequiredPowerUpDistance;
	
	UPROPERTY(EditDefaultsOnly,Category=PowerUp)
	int32 DesiredPowerupCount;
	// Delegate (event handler) that executes automatically when an asynchronous EQS query finishes processing.
	//TSharedPtr-> smart pointer that ensures the result data remains valid in memory while we use it.
	//FEnvQueryResult-> data returned by the Query
	//Bind this Function to FEnvQueryResult 
	void OnPowerUpSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);
	
	UFUNCTION()
	void SpawnBotTimerElapsed();
	
	UPROPERTY(EditAnywhere,Category=AI)
	float SpawnTimerInterval;
	
	UFUNCTION()//UEnvQueryInstanceBlueprintWrapper is the bridge that allows Blueprints to handle the results of an Environment Query System (EQS) test.
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,EEnvQueryStatus::Type QueryStatus);
	
	// GM does not do begin play cause it is responsible. of calling BP on all the classes.
public:
	
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void StartPlay() override;
	
	virtual void OnActorKilled(AActor*VictimActor,AActor*Killer);
	
	UFUNCTION()
	void RespawnPlayerElapsed(AController*Controller);
	
	UFUNCTION(Exec)
	void KillAI();
	
	UFUNCTION(BlueprintCallable,Category="SaveGame")
	void WriteSaveGame();
	
	void LoadSaveGame();
	
};
