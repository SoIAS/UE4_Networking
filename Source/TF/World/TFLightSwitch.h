// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/TFToggleable.h"
#include "TFLightSwitch.generated.h"

// Light switch that can toggle multiple different light actors (e.g. few point lights, directional lights) at once 
UCLASS()
class TF_API ATFLightSwitch : public ATFToggleable
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditInstanceOnly, Category = "Light")
	TArray<class ALight*> LightActors;

	UPROPERTY(EditAnywhere, Category = "Light")
	TArray<FComponentReference> LightComponents;

	void OnRep_IsToggled_Implementation() override;
};
