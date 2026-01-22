// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRPGPowerupActor.h"

// Sets default values
AActionRPGPowerupActor::AActionRPGPowerupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActionRPGPowerupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActionRPGPowerupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

