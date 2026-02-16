
#include "ActionRPGItemChest.h"
#include "NiagaraComponent.h"
#include "Components//StaticMeshComponent.h"



AActionRPGItemChest::AActionRPGItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	OpenChestEffect = CreateDefaultSubobject<UNiagaraComponent>("OpenChestEffect");
	OpenChestEffect->SetupAttachment(RootComponent);

	OpenChestEffect->bAutoActivate = false;
	OpenChestEffect->bAutoManageAttachment = true;

	TargetPitch = 110;
	/*Sets bReplicates to true, the actor will now be replicated from Server - > Clients */
	//SetReplicates(true);

}

void AActionRPGItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	/*
	bLidOpened = true;
	if (bLidOpened){OpenChest();}
	*/

	LidMesh->SetRelativeRotation(FRotator(TargetPitch,0,0));
}


void AActionRPGItemChest::OpenChest()
{

}
