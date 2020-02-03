 // Fill out your copyright notice in the Description page of Project Settings.


#include "TFDestroyable.h"
#include "UnrealNetwork.h"

#include "Engine/Engine.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

ATFDestroyable::ATFDestroyable()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshCompoentn"));
	SetRootComponent(StaticMeshComponent);
	
	CurrentState = 0;
	
	SetReplicates(true);
}

float ATFDestroyable::TakeDamage(
	const float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* const EventInstigator,
	AActor* const DamageCauser)
{
	if (Health <= 0.0f || DamageAmount < 0.0f)
	{
		return {};
	}

	const float DamageDealt = FMath::Min(Health, DamageAmount);
	Health -= DamageDealt;

	UpdateState();
	if (bDestroyOnZeroHealth && Health == 0)
	{
		Destroy();
	}

	return DamageDealt;
}

void ATFDestroyable::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	OnRep_CurrentState();
}

void ATFDestroyable::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// TODO, anyway to throw errors when bps gets compiled?
	if (DestructibleStates.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty destructible states"));
		return;
	}

	// Obviously we could either look for next state in unsorted array or sort it
	// For sorting, the array would need to be replicated
	// For ease let's just print an error
	float PreviousHealthValue = DestructibleStates[0].Health;
	for (int i = 1; i < DestructibleStates.Num(); ++i)
	{
		const auto CurrentHealthValue = DestructibleStates[i].Health;
		if (PreviousHealthValue <= DestructibleStates[i].Health)
		{
			UE_LOG(LogTemp, Error, TEXT("Destructible states are not ordered by health"));
			break;
		}

		PreviousHealthValue = CurrentHealthValue;
	}
}

void ATFDestroyable::BeginPlay()
{
	Super::BeginPlay();

 	if(Role == ROLE_Authority)
 	{
		if (CurrentState < DestructibleStates.Num())
		{
			Health = DestructibleStates[0].Health;
		}
 	}

	OnRep_CurrentState();
}

void ATFDestroyable::OnRep_CurrentState()
{
	// Check in case of an empty destructible states
	if (CurrentState < DestructibleStates.Num())
	{
		StaticMeshComponent->SetStaticMesh(DestructibleStates[CurrentState].StaticMesh);
	}
}

void ATFDestroyable::OnStateChange_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DestructibleStates[0].TransitionSound, GetActorLocation());
}

void ATFDestroyable::UpdateState()
{
	int NextState = CurrentState + 1;
	int NewState = CurrentState;
	while (
		NextState < DestructibleStates.Num() &&
		DestructibleStates[NextState].Health >= Health)
	{
		NewState = NextState;
		++NextState;
	}

	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		OnStateChange(); // spawn particle and sound only when state changes (do not spawn that for late joining players)
		OnRep_CurrentState(); // for listening server
	}
}

void ATFDestroyable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATFDestroyable, CurrentState);
	DOREPLIFETIME(ATFDestroyable, Health);
}