
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ActionRPGSaveGame.generated.h"

UCLASS()
class ACTIONRPG_API UActionRPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	int32 Credits;
	
	
	
};
