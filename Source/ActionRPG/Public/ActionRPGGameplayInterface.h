
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionRPGGameplayInterface.generated.h"


UINTERFACE(MinimalAPI)
class UActionRPGGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONRPG_API IActionRPGGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Interact(APawn*InstigatorPawn);
};
