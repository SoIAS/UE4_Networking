 // Fill out your copyright notice in the Description page of Project Settings.


#include "TFDestroyable.h"
#include "UnrealNetwork.h"

#include "Engine/Engine.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

ATFDestroyable::ATFDestroyable()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshCompoentn"));
	SetRootComponent(StaticMeshComponent);
	
	CurrentState = 0;
	
	SetReplicates(true);
}

void ATFDestroyable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Automatically update the mesh in blueprints and engine's viewport to first state
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
		if (PreviousHealthValue <= CurrentHealthValue)
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

 	if (Role == ROLE_Authority)
 	{
		if (CurrentState < DestructibleStates.Num())
		{
			Health = DestructibleStates[0].Health;
		}
	}

	OnRep_CurrentState();
}

float ATFDestroyable::TakeDamage(
	const float DamageAmount,
	FDamageEvent const& /*DamageEvent*/,
	AController* const /*EventInstigator*/,
	AActor* const /*DamageCauser*/)
{
	if (Health <= 0.0f || DamageAmount < 0.0f)
	{
		return {};
	}

	const float DamageDealt = FMath::Min(Health, DamageAmount);
	Health -= DamageDealt;
	OnRep_Health();

	UpdateState();
	if (bDestroyOnZeroHealth && Health == 0)
	{
		OnStateChange(CurrentState); // Play last state effects
		OnDestroyedOrLastState();
		Destroy();
	}
	else if (!bDestroyOnZeroHealth && CurrentState == DestructibleStates.Num() - 1)
	{
		// This was last state and this object is not going to get be Destroy'ed(), 
		// let's set the hp to 0, so this function is not going to pass initial check again
		Health = 0;
		OnDestroyedOrLastState();
	}

	return DamageDealt;
}

void ATFDestroyable::OnRep_CurrentState()
{
	if (CurrentState < DestructibleStates.Num())
	{
		StaticMeshComponent->SetStaticMesh(DestructibleStates[CurrentState].StaticMesh);
	}
}

void ATFDestroyable::OnStateChange_Implementation(const int PreviousState)
{
	const auto& State = DestructibleStates[PreviousState];

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotator = FRotator::ZeroRotator;
	if (const auto EffectLocationComponent = Cast<UArrowComponent>(State.EffectLocationArrowComponent.GetComponent(this)))
	{
		SpawnLocation = EffectLocationComponent->GetComponentLocation();
		SpawnRotator = EffectLocationComponent->GetComponentRotation();
	}
	
	UGameplayStatics::PlaySoundAtLocation(this, State.TransitionSound, SpawnLocation);
	UGameplayStatics::SpawnEmitterAtLocation(this, State.TransitionEffect, SpawnLocation, SpawnRotator, true);
}

void ATFDestroyable::OnRep_Health()
{
	OnHealthChanged.ExecuteIfBound();
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
		// Using multicast for visuals since the object might get destroyed right after this call
		OnStateChange(CurrentState); // spawn particle and sound only when state changes (do not spawn that for late joining players)
		CurrentState = NewState;
		OnRep_CurrentState(); // for listening server
	}
}

void ATFDestroyable::OnDestroyedOrLastState_Implementation()
{
	
}

void ATFDestroyable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATFDestroyable, CurrentState);
	DOREPLIFETIME(ATFDestroyable, Health);
}
