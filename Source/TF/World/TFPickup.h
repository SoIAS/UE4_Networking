// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/TFInteractable.h"
#include "TFPickup.generated.h"

/**
 * 
 */
UCLASS()
class TF_API ATFPickup : public ATFInteractable
{
	GENERATED_BODY()

public:
	ATFPickup();
	void BeginPlay() override;
	
	void OnUse(APawn* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* PickupSound;
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_OnPickedUp)
	bool bPickedUp;

	UFUNCTION()
	void OnRep_OnPickedUp();

	UFUNCTION(Reliable, NetMulticast)
	void PlayPickupSound();
};
