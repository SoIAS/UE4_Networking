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
	
	void OnUse(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* PickupSound;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<class ATFItem> ItemClass;
	
private:
	UFUNCTION(Reliable, NetMulticast)
	void NetMulticast_PlayPickupSound();
};
