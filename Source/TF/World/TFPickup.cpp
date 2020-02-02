// Fill out your copyright notice in the Description page of Project Settings.


#include "TFPickup.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Items/TFItem.h"
#include "TFCharacter.h"

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
	const auto TFPawn = Cast<ATFCharacter>(InstigatorPawn);
	if (!TFPawn || IsPendingKill())
	{
		return;
	}

	// todo, check if there is space in inventory to pickup, before actually spawning
	FActorSpawnParameters SpawnInfo{};
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ATFItem* const SpawnedItem = GetWorld()->SpawnActor<ATFItem>(ItemClass, SpawnInfo);

	if(!TFPawn->PickupItem(SpawnedItem))
	{
		SpawnedItem->Destroy();
		return;
	}
	
	PlayPickupSound();

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
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATFPickup, bPickedUp);
}
