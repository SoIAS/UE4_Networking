// Fill out your copyright notice in the Description page of Project Settings.


#include "TFPickup.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Items/TFItem.h"
#include "TFCharacter.h"

ATFPickup::ATFPickup()
{
	GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ATFPickup::OnUse(APawn* const InstigatorPawn)
{
	const auto TFPawn = Cast<ATFCharacter>(InstigatorPawn);
	if (!TFPawn || IsPendingKill())
	{
		return;
	}

	if (!TFPawn->CanPickupItem())
	{
		return;
	}

	FActorSpawnParameters SpawnInfo{};
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ATFItem* const SpawnedItem = GetWorld()->SpawnActor<ATFItem>(ItemClass, SpawnInfo);

	if (!TFPawn->PickupItem(SpawnedItem))
	{
		SpawnedItem->Destroy();
		return;
	}

	PlayPickupSound();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OBJECT USED"));
	Destroy();
}

void ATFPickup::PlayPickupSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
}