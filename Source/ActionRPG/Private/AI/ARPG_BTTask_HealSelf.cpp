
#include "AI/ARPG_BTTask_HealSelf.h"
#include "ActionRPGAttributeComponent.h"
#include "AIController.h"


EBTNodeResult::Type UARPG_BTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	
	UActionRPGAttributeComponent* AttributeComponent = UActionRPGAttributeComponent::GetAttributes(Pawn);
	if (ensure(AttributeComponent))
	{
		AttributeComponent->ApplyHealthChange(Pawn,AttributeComponent->GetMaxHealth());
		
	}
	return EBTNodeResult::Succeeded;
}
