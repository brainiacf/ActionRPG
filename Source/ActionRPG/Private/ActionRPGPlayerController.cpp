
#include "ActionRPGPlayerController.h"

void AActionRPGPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}
