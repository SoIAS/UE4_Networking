// Fill out your copyright notice in the Description page of Project Settings.


#include "TFPickup.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ATFPickup::ATFPickup()
{
	// I am 100% sure this is unnecessary since we got one in base class.
	SetReplicates(true);
}

void ATFPickup::BeginPlay()
{
	// todo, shoould I check for role?
	bPickedUp = false; // todo, remove, not used atm
}

void ATFPickup::OnUse(APawn* const InstigatorPawn)
{
	if (!InstigatorPawn || IsPendingKill())
	{
		return;
	}

	PlayPickupSound();
	
	// todo instigator->pickup(item);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OBJECT USED"));
	Destroy();
}

void ATFPickup::OnRep_OnPickedUp()
{
	
}

void ATFPickup::PlayPickupSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
}

void ATFPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ATFPickup, bPickedUp);
}
