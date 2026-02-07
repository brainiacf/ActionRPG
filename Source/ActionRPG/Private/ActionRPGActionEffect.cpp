
#include "ActionRPGActionEffect.h"
#include "ActionRPGActionComponent.h"


UActionRPGActionEffect::UActionRPGActionEffect()
{
	bAutoStart = true;
}


void UActionRPGActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	if (Duration>0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"StopAction",Instigator);
		GetWorld()->GetTimerManager().SetTimer(Duration_Handle,Delegate,Duration,false);
	}
	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"ExecutePeriodicEffect",Instigator);
		GetWorld()->GetTimerManager().SetTimer(Period_Handle,Delegate,Period,true);
		
		
	}
}

void UActionRPGActionEffect::StopAction_Implementation(AActor* Instigator)
{
																			// wow UE KINDA_SMALL_NUMBER-> (1.e-4f) 
	if (GetWorld()->GetTimerManager().GetTimerRemaining(Period_Handle) <  KINDA_SMALL_NUMBER)
	{
			ExecutePeriodicEffect(Instigator);
	}
	Super::StopAction_Implementation(Instigator);
	GetWorld()->GetTimerManager().ClearTimer(Period_Handle);
	GetWorld()->GetTimerManager().ClearTimer(Duration_Handle);
	
	UActionRPGActionComponent* ActionComponent = GetOwningComponent();
	if (ActionComponent)
	{
		ActionComponent->RemoveAction(this);
	}
	
	
	
	

}

void UActionRPGActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}

