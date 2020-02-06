// Fill out your copyright notice in the Description page of Project Settings.


#include "TFDoor.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealNetwork.h"

void ATFDoor::BeginPlay()
{
	OnRep_IsDoorOpen();
}

void ATFDoor::OnUse(APawn* const /*InstigatorPawn*/)
{
	bIsDoorOpen = !bIsDoorOpen;
	OnRep_IsDoorOpen();
}

void ATFDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATFDoor, bIsDoorOpen);
}
