// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/TFInteractable.h"
#include "TFToggleable.generated.h"

/**
 * 
 */
UCLASS()
class TF_API ATFToggleable : public ATFInteractable
{
	GENERATED_BODY()
public:
	ATFToggleable();
	void BeginPlay() override;
	void OnUsed(APawn* InstigatorPawn) override;

protected:
	void InitBasedOnIsToggled();

	UPROPERTY(BlueprintReadOnly)
	bool bInitFinished;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_IsToggled)
	bool bIsToggled;

	UFUNCTION(BlueprintNativeEvent)
	void OnRep_IsToggled();
};