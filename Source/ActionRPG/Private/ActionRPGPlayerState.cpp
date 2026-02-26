#include "ActionRPGPlayerState.h"

#include "ActionRPGSaveGame.h"
#include "Net/UnrealNetwork.h"

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


void AActionRPGPlayerState::OnRep_Credits(int32 OldCredits)
{
	int32 Delta = Credits - OldCredits;
	OnCreditsChanged.Broadcast(this,Credits,Delta);
}

int32 AActionRPGPlayerState::GetCredits() const
{
	return Credits;
	
}
void AActionRPGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AActionRPGPlayerState, Credits);
}

void AActionRPGPlayerState::SavePlayerState_Implementation(UActionRPGSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
	
}

void AActionRPGPlayerState::LoadPlayerState_Implementation(UActionRPGSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;
	}
}


