#include "ActionRPGAttributeComponent.h"

#include "ActionRPGAction.h"
#include "ActionRPGGameModeBase.h"
#include "Net/UnrealNetwork.h"

// console variable to manipulate Damage while the game is running, Use .GetValueOnGameThread() to get the var
static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"),1.0f,TEXT("MultiplyDamage"),ECVF_Cheat);



UActionRPGAttributeComponent::UActionRPGAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	MaxHealth = 100.0f;   Health =MaxHealth;
	MaxRage = 100.0f; Rage = 0.0f;
	
	SetIsReplicatedByDefault(true);

}
bool UActionRPGAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float DeltaHealth)
{
	if (!GetOwner()->CanBeDamaged() && DeltaHealth <0.0f)  
	{
		return false;
	}
	
	if (DeltaHealth < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		DeltaHealth*=DamageMultiplier;
	} 
	
	float OldHealth = Health;

	float NewHealth = Health = FMath::Clamp(Health + DeltaHealth,0.0f,MaxHealth); 	
	
	float ActualDelta = NewHealth - OldHealth;
	//IsServer()
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;	
		
		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor,Health,ActualDelta);	
		}
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			AActionRPGGameModeBase* GM = GetWorld()->GetAuthGameMode<AActionRPGGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(),InstigatorActor);
			}
		}
	
	}
	
	return ActualDelta != 0.0f;

}

//helper function -> to quickly get the Attribute Component from Any actor  
UActionRPGAttributeComponent* UActionRPGAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor) {return Cast<UActionRPGAttributeComponent>(FromActor->GetComponentByClass(UActionRPGAttributeComponent::StaticClass()));}
	return nullptr;
}

bool UActionRPGAttributeComponent::ISActorAlive(AActor* Actor)
{
	UActionRPGAttributeComponent* AttributeComponent = GetAttributes(Actor);
	
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}
	return false;
}
void UActionRPGAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UActionRPGAttributeComponent, Health);
	DOREPLIFETIME(UActionRPGAttributeComponent, MaxHealth);
	DOREPLIFETIME(UActionRPGAttributeComponent, MaxRage);
	DOREPLIFETIME(UActionRPGAttributeComponent, Rage);
	
	//DOREPLIFETIME_CONDITION -> can be used for bandwidth optimization since, we only need to send the data when it is absolutely needed. basically getting the vars with condition will help in bandwidth send when needed
	//DOREPLIFETIME_CONDITION(UActionRPGAttributeComponent,MaxHealth,COND_OwnerOnly);
}
void UActionRPGAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth,
	float Delta)
{
	OnHealthChange.Broadcast(InstigatorActor,this,NewHealth,Delta);	
}
void UActionRPGAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage,
	float ActualDelta)
{
	OnRageChange.Broadcast(InstigatorActor,this,NewRage,ActualDelta);
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool UActionRPGAttributeComponent::IsFullRage() const
{
	return Rage == MaxRage;
}

float UActionRPGAttributeComponent::GetMaxRage() const
{
	return MaxRage;
}

float UActionRPGAttributeComponent::GetRage() const
{
	return Rage;
}



bool UActionRPGAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor,-GetMaxHealth());
}

bool UActionRPGAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float DeltaRage)
{
	if (!GetOwner()->CanBeDamaged() && DeltaRage <0.0f) // can we damage the owner , if not return here 
	{
		return false;
	}
	float OldRage = Rage;
	Rage = FMath::Clamp(Rage + DeltaRage,0.0f,MaxRage);
	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		//OnRageChange.Broadcast(InstigatorActor,this,Rage,ActualDelta);
		MulticastRageChanged_Implementation(InstigatorActor,Rage,ActualDelta);
		return true;
	}
	return false;
}



bool UActionRPGAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

float UActionRPGAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool UActionRPGAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float UActionRPGAttributeComponent::GetHealth() const
{
	return Health;
}
