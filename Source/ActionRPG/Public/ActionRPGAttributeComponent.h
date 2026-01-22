
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionRPGAttributeComponent.generated.h"

//Declare dynamic multicast delegate fiveparams -> ue macro that declares a blueprint assignable multicast delegate
//In Unreal, the F prefix is a catch-all for "Any class or struct that is NOT a UObject or an Actor."
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChange,AActor*,InstigatorActor,UActionRPGAttributeComponent*,OwningComponent,float,NewHealth,float,Delta);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONRPG_API UActionRPGAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category=Attributes)
	static UActionRPGAttributeComponent*GetAttributes(AActor* FromActor);
	UFUNCTION(BlueprintCallable,Category=Attributes)
	static bool ISActorAlive(AActor*Actor);

public:	
	UActionRPGAttributeComponent();
	// funcS are const because in BPs it creates a Pure Fucntions to retrieve or Calc Data.
	// for const in C++ it tells the compiler that it will not change the member variable. 
	UFUNCTION(BlueprintCallable,Category=Attributes)
	bool ApplyHealthChange(AActor*InstigatorActor,float DeltaHealth);
	
	UFUNCTION(BlueprintCallable,Category=Attributes)
	bool IsFullHealth() const;
	
	UFUNCTION(BlueprintCallable,Category=Attributes)
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable,Category=Attributes)
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable,Category=Attributes)
	float GetHealth() const;
	
	UPROPERTY(BlueprintAssignable,Category=Events)
	FOnHealthChange OnHealthChange;

protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Attributes)
	float Health;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Attributes)
	float MaxHealth;

public:	
	
	UFUNCTION(BlueprintCallable,Category=Attributes)
	bool Kill(AActor*InstigatorActor);

		
};
