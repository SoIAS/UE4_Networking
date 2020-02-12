// Fill out your copyright notice in the Description page of Project Settings.


#include "TFToggleable.h"
#include "UnrealNetwork.h"

ATFToggleable::ATFToggleable()
{
	bInitFinished = false;
}

void ATFToggleable::BeginPlay()
{
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
	// This boolean wouldn't be necessary if we were using replicate static mesh && replicate movement (to show e.g. openeed/closed door)
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