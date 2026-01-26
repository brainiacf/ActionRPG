#include "ActionRPGGameModeBase.h"

#include "ActionRPGActionComponent.h"
#include "AI/ActionRPGAICharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EngineUtils.h"
#include "ActionRPGAttributeComponent.h"
#include "ActionRPGCharacter.h"
#include "DrawDebugHelpers.h"
#include "EnvironmentQuery/EnvQuery.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning bots"),ECVF_Cheat);

AActionRPGGameModeBase::AActionRPGGameModeBase()
{
	SpawnTimerInterval = 2.0f;
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



void AActionRPGGameModeBase::KillAI()
{
	for (TActorIterator<AActionRPGAICharacter>It(GetWorld()); It; ++It)
	{
		AActionRPGAICharacter* Bot = *It;
		UActionRPGAttributeComponent *AttributeComponent = UActionRPGAttributeComponent::GetAttributes((Bot));
		if (ensure(AttributeComponent)&& AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this);
		}
		
	}
}
void AActionRPGGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Log,TEXT("bot spawning "));
		return;
	}
	UEnvQueryInstanceBlueprintWrapper*QueryInstance = UEnvQueryManager::RunEQSQuery(this,
		SpawnBotsQuery,this,
		EEnvQueryRunMode::RandomBest5Pct,nullptr);
	int32 NrOfAliveBots = 0;
	for (TActorIterator<AActionRPGAICharacter> It(GetWorld()); It; ++It)
	{
		AActionRPGAICharacter* Bot = *It;
		UActionRPGAttributeComponent *AttributeComponent = UActionRPGAttributeComponent::GetAttributes((Bot));
		if (ensure(AttributeComponent)&& AttributeComponent->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	UE_LOG(LogTemp,Log,TEXT("No.OfAliveBots: %d"),NrOfAliveBots);
	float MaxBotCount = 10.0f;
	if (DifficultyCurve){MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);}
	if (NrOfAliveBots > MaxBotCount){return;}
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&AActionRPGGameModeBase::OnQueryCompleted);
	}
}
/*hola amigo kaise ho theek ho?*/

void AActionRPGGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("Query Incomplete"));
	}
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0],FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(),Locations[0],100,20,FColor::Red);
	}
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

void AActionRPGGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}
