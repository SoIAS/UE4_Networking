// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/TFInteractable.h"
#include "TFDoor.generated.h"

/**
 * 
 */
UCLASS()
class TF_API ATFDoor : public ATFInteractable
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	
	void OnUse(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_IsDoorOpen, BlueprintReadOnly)
	bool bIsDoorOpen;

	UFUNCTION(BlueprintImplementableEvent)
	void OnRep_IsDoorOpen();
};
