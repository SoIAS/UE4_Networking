// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/TFDestroyable.h"
#include "TFDestructibleContainer.generated.h"

/**
 * 
 */
UCLASS()
class TF_API ATFDestructibleContainer : public ATFDestroyable
{
	GENERATED_BODY()


	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATFPickup> PickupClass;
	
	void OnDestroyedOrLastState_Implementation() override;
	
};
