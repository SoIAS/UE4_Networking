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

	UPROPERTY(EditDefaultsOnly)
	FComponentReference EffectLocationArrowComponent;
};

// todo, rename to destructible
UCLASS()
class TF_API ATFDestroyable : public AActor
{
	GENERATED_BODY()
	
public:	
	ATFDestroyable();

	FORCEINLINE bool DestroyOnZeroHealth() const noexcept
	{
		return bDestroyOnZeroHealth;
	}
	
	FORCEINLINE float GetHealth() const noexcept
	{
		return Health;
	}
	
protected:
	void OnConstruction(const FTransform& Transform) override;
	void PostInitializeComponents() override;
	void BeginPlay() override;

public:
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TArray<FDestructibleState> DestructibleStates;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentState)
	int CurrentState;

	UFUNCTION()
	void OnRep_CurrentState();

	UFUNCTION(Reliable, NetMulticast)
	void OnStateChange(int PreviousState);

	UPROPERTY(EditDefaultsOnly)
	bool bDestroyOnZeroHealth;

	UPROPERTY(Replicated)
	float Health;

	void UpdateState();

	UFUNCTION(BlueprintNativeEvent)
	void OnDestroyedOrLastState();
};