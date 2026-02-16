
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionRPGGameplayInterface.h"
#include "ActionRPGItemChest.generated.h"


class UNiagaraComponent;
class UStaticMeshComponent;
UCLASS()
class ACTIONRPG_API AActionRPGItemChest : public AActor, public  IActionRPGGameplayInterface
{
	GENERATED_BODY()

public:
	AActionRPGItemChest();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;



protected:
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	UPROPERTY(EditDefaultsOnly,Category=Animation)
	TObjectPtr<UCurveFloat> LidAnimCurve;

	UPROPERTY(EditAnywhere)
	bool bLidOpened;

	void OpenChest();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LidMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> OpenChestEffect;

};
