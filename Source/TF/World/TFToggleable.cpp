// Fill out your copyright notice in the Description page of Project Settings.


#include "TFToggleable.h"
#include "UnrealNetwork.h"

void ATFToggleable::BeginPlay()
{
	// Update to the initial state of bIsToggled (set per instance basis) on both server and clients
	OnRep_IsToggled();
}

void ATFToggleable::OnUse(APawn* const /*InstigatorPawn*/)
{
	bIsToggled = !bIsToggled;
	OnRep_IsToggled();
}

void ATFToggleable::OnRep_IsToggled_Implementation()
{
	
}

void ATFToggleable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATFToggleable, bIsToggled);
}