
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionRPGInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONRPG_API UActionRPGInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActionRPGInteractionComponent();
	
	void PrimaryInteract();



		
};
