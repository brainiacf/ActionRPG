
#pragma once

#include "CoreMinimal.h"
#include "ActionRPGPowerupActor.h"
#include "ActionRPGPowerupHealthPotion.generated.h"

class UMeshComponent;
/**
 * 
 */
UCLASS()
class ACTIONRPG_API AActionRPGPowerupHealthPotion : public AActionRPGPowerupActor
{
	GENERATED_BODY()
public:
	AActionRPGPowerupHealthPotion();
	
protected:
	UPROPERTY(EditAnywhere,Category=HealthPotion)
	TObjectPtr<UMeshComponent> MeshComponent;
	
	void Interact_Implementation(APawn* InstigatorPawn) override;
	
};
