// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFDestroyable.generated.h"

USTRUCT(BlueprintType)
struct FDestructibleState
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* TransitionSound;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* TransitionEffect;

};

// todo, rename to destructible
UCLASS()
class TF_API ATFDestroyable : public AActor
{
	GENERATED_BODY()
	
public:	
	ATFDestroyable();

	FORCEINLINE float GetHealth() const noexcept
	{
		return Health;
	}

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	void OnConstruction(const FTransform & Transform) override;
	void PostInitializeComponents();
	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TArray<FDestructibleState> DestructibleStates;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentState)
	int CurrentState;

	UFUNCTION()
	void OnRep_CurrentState();

	UFUNCTION(Reliable, NetMulticast)
	void OnStateChange();

	UPROPERTY(EditDefaultsOnly)
	bool bDestroyOnZeroHealth; // todo

	UPROPERTY(Replicated)
	float Health;

	void UpdateState();
};