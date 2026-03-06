
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ActionRPGSaveGame.generated.h"


USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()
	
public:
	/*to identify the actor*/
	UPROPERTY()
	FString ActorName; 
	
	UPROPERTY()
	FTransform Transform;
	
	UPROPERTY()
	TArray<uint8> ByteData;
	
};




UCLASS()
class ACTIONRPG_API UActionRPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	int32 Credits;
	
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
	
};
