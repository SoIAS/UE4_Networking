// Fill out your copyright notice in the Description page of Project Settings.


#include "TFPickup.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"

ATFPickup::ATFPickup()
{
	// I am 100% sure this is unnecessary since we got one in base class.
	SetReplicates(true);
}

void ATFPickup::BeginPlay()
{
	// todo, shoould I check for role?
	bPickedUp = false;
}

void ATFPickup::OnUse(APawn* const UsedBy)
{
	// recheck
	if (IsPendingKill() || bPickedUp)
	{
		return;
	}

	bPickedUp = true;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OBJECT USED"));
	Destroy();
}

void ATFPickup::OnRep_OnPickedUp()
{
	// e.g play sound
}

void ATFPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ATFPickup, bPickedUp);
}