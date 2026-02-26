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
#include "ActionRPGPlayerState.h"
#include "ActionRPGSaveGame.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/AssetManager.h"
#include "Logging/StructuredLog.h"
#include "ActionRPGSaveGame.h"




static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning bots"),ECVF_Cheat);


AActionRPGGameModeBase::AActionRPGGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 50.0f;
	PlayerStateClass = AActionRPGPlayerState::StaticClass();
	
	DesiredPowerupCount = 10;
	RequiredPowerUpDistance = 20000.0f;
	InitialSpawnCredits = 50;
	
	SlotName = "SaveGame01";
	
}

void AActionRPGGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
	
}




void AActionRPGGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,
		this,
		&AActionRPGGameModeBase::SpawnBotTimerElapsed,
		SpawnTimerInterval,
		true);
	// atleast have one powerUp class
	if (ensure(PowerUpClasses.Num()>0))
	{
		// skip blueprint wrapper and use the direct C++ option which the wrapper uses as well 
		//FEnvQueryRequest Request( prepares a new query using PowerUpSpawnQuery asset[which are basically rules for the search] and sets this actor as the owner/requester)
		FEnvQueryRequest Request(PowerUpSpawnQuery,this);
		//Request.Execute()-> Instantly launches the query on a background thread.
		//EEnvQueryRunMode::AllMatching-> tells the query to return every valid Item it finds, rather than just the single best score 
		Request.Execute(EEnvQueryRunMode::AllMatching,this,&ThisClass::OnPowerUpSpawnQueryCompleted);
		//&ThisClass::OnPowerUpSpawnQueryCompleted -> registers the function as the callback
	}
	
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


void AActionRPGGameModeBase::OnPowerUpSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		UE_LOG(LogTemp,Warning,TEXT("Failed to spawn bot spawn"));
		return;
	}
	// Retrieve all possible locations where powerup instances could spawn 
	TArray<FVector> Locations;
	
	QueryResult->GetAllAsLocations(Locations); // Query result fills the Locations Array with the all valid spots
	
	// keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;
	int32 SpawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerupCount && Locations.Num()>0)//
	{
		// pick a random Index from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0,Locations.Num()-1);
		
		FVector PickedLocation = Locations[RandomLocationIndex];
		//remove the picked location so that it isn't processed twice 
		Locations.RemoveAt(RandomLocationIndex);
		//check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();
			
			if (DistanceTo < RequiredPowerUpDistance)
			{
				// show skiped location due to distance 
				bValidLocation = false;
				break;
			}
		}
		// failed the distance Test 
		if (!bValidLocation)
		{
			continue; 
		}
		//pick random clas index from the PowerupClasses Arrays
		int32 RandomClassIndex = FMath::RandRange(0,PowerUpClasses.Num()-1);
		
		TSubclassOf<AActor> RandomPowerupClass = PowerUpClasses[RandomClassIndex];
		GetWorld()->SpawnActor<AActor>(RandomPowerupClass,PickedLocation,FRotator::ZeroRotator);
		
		// 
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
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
	//to give credits for kill
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		AActionRPGPlayerState*PS = KillerPawn->GetPlayerState<AActionRPGPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);
		}
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void AActionRPGGameModeBase::WriteSaveGame()
{
	// Iterate all players states, we don't have proper ID to match yet (requires Steam or EOS )
	for(int32 i = 0; i < GameState->PlayerArray.Num(); i++ )
	{
		AActionRPGPlayerState* PS = Cast<AActionRPGPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			 PS->SavePlayerState(CurrentSaveGame);
			break;
		
		}
	}
	
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SlotName,0);
}

void AActionRPGGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		CurrentSaveGame =  Cast<UActionRPGSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName,0)) ;
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp,Warning,TEXT("No SaveGame for ")); 
			return; 
		}
		UE_LOG(LogTemp,Log,TEXT("Loaded SaveGame for "));
	}
	else
	{
		CurrentSaveGame = Cast<UActionRPGSaveGame>(UGameplayStatics::CreateSaveGameObject(UActionRPGSaveGame::StaticClass())) ; 
		
		UE_LOG(LogTemp,Log,TEXT("Created New Save Game "));
	}
}

void AActionRPGGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	
	AActionRPGPlayerState* PS = NewPlayer->GetPlayerState<AActionRPGPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
	
}
