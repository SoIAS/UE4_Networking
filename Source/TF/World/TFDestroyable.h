// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFDestroyable.generated.h"

UENUM(BlueprintType)
enum class EDestroyableState : uint8
{
	Intact,
	Damaged,
	Destroyed
};

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

	FORCEINLINE EDestroyableState GetDestroyableState() const noexcept
	{
		return DestroyableState;
	}

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DefaultHealth;

	UPROPERTY(Replicated)
	float Health;

	UPROPERTY(ReplicatedUsing = OnRep_DestroyableState, BlueprintReadOnly)
	EDestroyableState DestroyableState;

	UFUNCTION()
	void OnRep_DestroyableState();
	
	void UpdateDestroyableState();
	void SetNewDestroyableState(EDestroyableState NewState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroyableStateChanged();
};