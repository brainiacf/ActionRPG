#include "ActionRPGAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UActionRPGAction_ProjectileAttack::UActionRPGAction_ProjectileAttack()
{
	//Set Defaults
	AttackAnimDelay = 0.2f;
	HandSocketName = "Muzzle_01";
}

void UActionRPGAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	//Ensure the Instigator is actually a character.
	ACharacter *Character = Cast<ACharacter>(Instigator);
	if (!Character){UE_LOG(LogTemp,Log,TEXT("dude where is your character ")); return; }
	
	if (Character)
	{
		// 1. play Visual Cues(Animation and particles)
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffects,
			Character->GetMesh(),
			HandSocketName,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget);
		// 2. Set a timer to spawn the actual projectile later
		// This ensures the projectile appears when the character actually "fires" in the animation
		if (Character->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this,"AttackAnimDelay_Elapsed",Character);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay,Delegate,AttackAnimDelay,false);
		}
		
	}
}

void UActionRPGAction_ProjectileAttack::AttackAnimDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	// safety check please assign this class in editor mf 
	if (ensure(ProjectileClass))
	{
		//start location from which the projectile will eject
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = InstigatorCharacter;
		
		//Do sphere trace to make hitting target easy since this is a Projectile and not a Bullet(LineTrace)
		// Trace 5000units forward from the camera rotation
		FHitResult HitResult;
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector()*5000.0f);
		
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		
		//Query params to not hit the player when we spawn the projectile 
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);
		
		// what objects block the targeting trace (static,dynamic,and pawn, enemies)
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		
		FRotator ProjectileRotation; 
		//FQuat::Identity -> is a constant that represents no rotation
		//Trace begin
		if (GetWorld()->SweepSingleByObjectType(HitResult,TraceStart,TraceEnd,FQuat::Identity,ObjectQueryParams,Shape,Params))
		{
			//Hit: CalC rotation from Hand-> ImpactPoint,this ensures accuracy even if camera is offset from the character 
			ProjectileRotation = FRotationMatrix::MakeFromX(HitResult.ImpactPoint-HandLocation).Rotator();
		}
		else
		{
			//Miss: CalC rotation from Hand-> Max Trace Distance
			//Fallback if we look at the sky or empty space 
			ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd-HandLocation).Rotator();
		}
		//
		//Spawn Projectile with the calculated rotation and hand location
		FTransform SpawnTransform = FTransform(ProjectileRotation,HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass,SpawnTransform,SpawnParameters);

	}
	// hello Action
	StopAction(InstigatorCharacter);


}

