// Fill out your copyright notice in the Description page of Project Settings.


#include "TFDestructibleContainer.h"
#include "Engine/World.h"
#include "TFStaticLibrary.h"
#include "World/TFPickup.h"

void ATFDestructibleContainer::OnDestroyedOrLastState_Implementation()
{
	const FVector SpawnLocation = UTFStaticLibrary::GetDropItemLocation(*this, { this });

	FActorSpawnParameters SpawnInfo{};
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	const auto DroppedItem = GetWorld()->SpawnActor<ATFPickup>(PickupClass, SpawnLocation, FRotator::ZeroRotator, SpawnInfo);

	if (DroppedItem)
	{
		// Lets play drop sound the same as the pickup one (just for the gist of it)
		DroppedItem->NetMulticast_PlayPickupSound();
	}
}