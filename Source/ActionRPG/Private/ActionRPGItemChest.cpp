
#include "ActionRPGItemChest.h"
#include "NiagaraComponent.h"
#include "Components//StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"


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
	SetReplicates(true);

}

void AActionRPGItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	/*
	bLidOpened = true;
	if (bLidOpened){OpenChest();}
	*/
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();// it runs auto on clients but for server you'll have to call it 
}


void AActionRPGItemChest::OnRep_LidOpened() 
{
	
	float CurrPitch = bLidOpened ?TargetPitch : 0.0f ;
	LidMesh->SetRelativeRotation(FRotator(TargetPitch,0,0));
	
}

void AActionRPGItemChest::OpenChest()
{

}

void AActionRPGItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AActionRPGItemChest, bLidOpened);
	
	
}
