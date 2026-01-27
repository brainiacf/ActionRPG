#include "AI/ARPG_BTTask_RangeAttack.h"
#include "AIController.h"
#include "PhysicsAssetUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "ActionRPGAttributeComponent.h"
#include "AI/ActionRPGAIController.h"

UARPG_BTTask_RangeAttack::UARPG_BTTask_RangeAttack()
{
	MaxBulletSpead = 2.0f;
}

EBTNodeResult::Type UARPG_BTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr) { return EBTNodeResult::Failed; }

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr) { return EBTNodeResult::Failed; }

		if (!UActionRPGAttributeComponent::ISActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(-MaxBulletSpead, MaxBulletSpead);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpead, MaxBulletSpead);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;
		
		AActor*NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass,MuzzleLocation,MuzzleRotation,Params);
		
		return NewProjectile? EBTNodeResult::Succeeded: EBTNodeResult::Failed;
		
		
	}
	return EBTNodeResult::Failed;
}
