// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/TFInteractable.h"
#include "TFLamp.generated.h"


// Simple UPointLight lamp
UCLASS()
class TF_API ATFLamp : public ATFInteractable
{
	GENERATED_BODY()

public:
	ATFLamp();

	void BeginPlay() override;
	
	void OnUse(APawn* InstigatorPawn) override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Light")
	class UPointLightComponent* PointLightComponent;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsLit)
	bool bIsLit;

	UFUNCTION()
	void OnRep_IsLit();
};
