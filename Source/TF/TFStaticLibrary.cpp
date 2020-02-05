// Fill out your copyright notice in the Description page of Project Settings.


#include "TFStaticLibrary.h"
#include "CollisionQueryParams.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

FVector UTFStaticLibrary::GetDropItemLocation(const AActor& Instigator, const TArray<AActor*>& IgnoredActors)
{
	constexpr auto MaxDropDistance{ 2000.f };
	
	const FVector TraceBegin{ Instigator.GetActorLocation() };
	const FVector TraceEnd{ TraceBegin + FVector::DownVector * MaxDropDistance };

	FCollisionQueryParams TraceParams{ "ItemDropTrace", false };
	TraceParams.AddIgnoredActors(IgnoredActors);

	FHitResult HitResult{};
	Instigator.GetWorld()->LineTraceSingleByChannel(HitResult, TraceBegin, TraceEnd, ECC_Visibility, TraceParams);

	return HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
}
