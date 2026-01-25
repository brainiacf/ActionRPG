#include "ActionRPGAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UActionRPGAction_ProjectileAttack::UActionRPGAction_ProjectileAttack()
{
	AttackAnimDelay = 0.2f;
	HandSocketName = "Muzzle_01";
}

void UActionRPGAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	ACharacter *Character = Cast<ACharacter>(Instigator);
	if (!Character){UE_LOG(LogTemp,Log,TEXT("dude where is your character ")); return; }
	
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffects,
			Character->GetMesh(),
			HandSocketName,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget);
		
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"AttackAnimDelay_Elapsed",Character);
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay,Delegate,AttackAnimDelay,false);
	}
}

void UActionRPGAction_ProjectileAttack::AttackAnimDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = InstigatorCharacter;
		
		FHitResult HitResult;
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector()*5000.0f);
		
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		
		//ignore player 
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);
		
		
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		
		FRotator ProjectileRotation; 
		//FQuat::Identity -> is a constant that represents no rotation
		
		if (GetWorld()->SweepSingleByObjectType(HitResult,TraceStart,TraceEnd,FQuat::Identity,ObjectQueryParams,Shape,Params))
		{
			ProjectileRotation = FRotationMatrix::MakeFromX(HitResult.ImpactPoint-HandLocation).Rotator();
		}
		else
		{
			ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd-HandLocation).Rotator();
		}
		FTransform SpawnTransform = FTransform(ProjectileRotation,HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass,SpawnTransform,SpawnParameters);

	}

	StopAction(InstigatorCharacter);


}

