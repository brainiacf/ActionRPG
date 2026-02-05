#include "ActionRPGPlayerState.h"

void AActionRPGPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta>0))
	{
		return;
	}
	Credits += Delta;
	OnCreditsChanged.Broadcast(this,Credits,Delta);
}

bool AActionRPGPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta>0))
	{
		return false;
	}
	if (Credits < Delta)
	{
		return false;
	}
	Credits -= Delta;
	OnCreditsChanged.Broadcast(this,Credits,Delta);
	return true;
}

int32 AActionRPGPlayerState::GetCredits() const
{
	return Credits;
	
}

