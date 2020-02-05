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
	GetWorld()->SpawnActor<ATFPickup>(PickupClass, SpawnLocation, FRotator::ZeroRotator, SpawnInfo);
}