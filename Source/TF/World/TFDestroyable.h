// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFDestroyable.generated.h"


// Delegate to notify players who looking at this object, that it's health changed, so we can update widget
// Of course we could notify the widget about its current state in many ways
// But I think using a delegate to notify ONLY when the actor is actually looking at this object, gives the best performance
DECLARE_DELEGATE(OnHealthChangedDelegate);

USTRUCT(BlueprintType)
struct FDestructibleState
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
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
UCLASS(Abstract)
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

	OnHealthChangedDelegate OnHealthChanged;
	
protected:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDestructibleState> DestructibleStates;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentState, BlueprintReadOnly)
	int CurrentState;

	UFUNCTION()
	void OnRep_CurrentState();

	UFUNCTION(Reliable, NetMulticast)
	void OnStateChange(int PreviousState);

	UPROPERTY(EditDefaultsOnly)
	bool bDestroyOnZeroHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;
	
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly)
	float Health;

	UFUNCTION()
	void OnRep_Health();

	void UpdateState();

	UFUNCTION(BlueprintNativeEvent)
	void OnDestroyedOrLastState();
};