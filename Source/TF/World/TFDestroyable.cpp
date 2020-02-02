 // Fill out your copyright notice in the Description page of Project Settings.


#include "TFDestroyable.h"
#include "UnrealNetwork.h"

ATFDestroyable::ATFDestroyable()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultHealth = 100;
	
	SetReplicates(true);
}

float ATFDestroyable::TakeDamage(
	const float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* const EventInstigator,
	AActor* const DamageCauser)
{
	if (Health <= 0.0f || DestroyableState == EDestroyableState::Destroyed)
	{
		return {};
	}

	// We are not healing the object, at least that's the consensus
	if (DamageAmount < 0.0f)
	{
		return {};
	}

	const float DamageDealt = FMath::Min(Health, DamageAmount);
	Health -= DamageDealt;

	UpdateDestroyableState();

	return DamageDealt;
}

void ATFDestroyable::BeginPlay()
{
	Super::BeginPlay();

 	if(Role == ROLE_Authority)
 	{
        Health = DefaultHealth;
 	}

 	// todo, set initial state?
	OnRep_DestroyableState();
}

void ATFDestroyable::OnRep_DestroyableState()
{
	OnDestroyableStateChanged();
}

void ATFDestroyable::UpdateDestroyableState()
{
	if (Health == DefaultHealth)
	{
		SetNewDestroyableState(EDestroyableState::Intact);
	}
	else if (Health == 0)
	{
		SetNewDestroyableState(EDestroyableState::Destroyed);
	}
	else
	{
		SetNewDestroyableState(EDestroyableState::Damaged);
	}
}

void ATFDestroyable::SetNewDestroyableState(EDestroyableState NewState)
{
	if (DestroyableState != NewState)
	{
		DestroyableState = NewState;
	}
}

void ATFDestroyable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATFDestroyable, Health);
	DOREPLIFETIME(ATFDestroyable, DestroyableState);
}