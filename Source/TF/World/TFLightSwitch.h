// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/TFInteractable.h"
#include "TFLightSwitch.generated.h"

// Light switch that can toggle multiple different light actors (e.g. few point lights, directional lights) at once 
UCLASS()
class TF_API ATFLightSwitch : public ATFInteractable
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;
	void OnUse(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditInstanceOnly, Category = "Light")
	TArray<class ALight*> LightActors;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_bIsLit)
	bool bIsLit;

	UFUNCTION()
	void OnRep_bIsLit();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLightToggled();
};
