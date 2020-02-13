// Fill out your copyright notice in the Description page of Project Settings.


#include "TFToggleable.h"
#include "UnrealNetwork.h"

ATFToggleable::ATFToggleable()
{
	bInitFinished = false;
}

void ATFToggleable::BeginPlay()
{
	Super::BeginPlay();
	
	// Update to the initial state of bIsToggled (set per instance basis) on both server and clients
	InitBasedOnIsToggled();
}

void ATFToggleable::OnUsed(APawn* const InstigatorPawn)
{
	bIsToggled = !bIsToggled;
	OnRep_IsToggled();

	Super::OnUsed(InstigatorPawn);
}

void ATFToggleable::InitBasedOnIsToggled()
{
	// Just a hack, could be easily replaced with multicast
	// this boolean wouldn't be necessary if we were using multicast called in OnUsed to show particle effects etc
	// But, since we are doing it this way, lets make sure effects are played only when init was done
	OnRep_IsToggled();
	bInitFinished = true;
}

void ATFToggleable::OnRep_IsToggled_Implementation()
{
	
}

void ATFToggleable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATFToggleable, bIsToggled);
}