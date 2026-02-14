
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionRPGInteractionComponent.generated.h"


class UActionRPGWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONRPG_API UActionRPGInteractionComponent : public UActorComponent
{/**/
	GENERATED_BODY()

public:	
	UActionRPGInteractionComponent();
	
	void PrimaryInteract();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	void FindBestInteractable();
	
	UFUNCTION(Server,Reliable)
	/*Reliable -> will always arrive, eventually. Request will be resent unless an acknowledgement was received
	Unreliable -> Not guaranteed, packet can get lost and wont retry*/
	void ServerInteract(AActor* InFocus);

	UPROPERTY()		
	AActor*FocusedActor;
	
	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UActionRPGWorldUserWidget> DefaultWidgetClass;
	
	UPROPERTY()
	UActionRPGWorldUserWidget* DefaultWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly,Category=Trace)
	float TraceDistance;
	
	UPROPERTY(EditDefaultsOnly,Category=Trace)
	float TraceRadius;
	
	UPROPERTY(EditDefaultsOnly,Category=Trace)
	TEnumAsByte<ECollisionChannel> CollisionChannel; 
};
