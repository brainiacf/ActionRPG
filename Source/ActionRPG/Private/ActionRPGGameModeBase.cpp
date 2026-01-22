#include "ActionRPGGameModeBase.h"
#include "AI/ActionRPGAICharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EngineUtils.h"
#include "ActionRPGAttributeComponent.h"
#include "ActionRPGCharacter.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning bots"),ECVF_Cheat);

AActionRPGGameModeBase::AActionRPGGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AActionRPGGameModeBase::SpawnBotTimerElapsed()
{
}

void AActionRPGGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
}

void AActionRPGGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,
		this,
		&AActionRPGGameModeBase::SpawnBotTimerElapsed,
		SpawnTimerInterval,
		true);
	
}

void AActionRPGGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AActionRPGCharacter*Player = Cast<AActionRPGCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerElapsed",Player->GetController());
		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,Delegate,RespawnDelay,false);
	}
	UE_LOG(LogTemp,Log,TEXT("OnAction Killed: Victim: %s,Killer: %s"),*GetNameSafe(VictimActor),*GetNameSafe(Killer));
}

void AActionRPGGameModeBase::KillAI()
{
//	for (TActorIterator<AActionRPGAICharacter>It(GetWorld()); It; ++It)
//	{
//		AActionRPGCharacter* Bot = *It;
//		
//	}
}
