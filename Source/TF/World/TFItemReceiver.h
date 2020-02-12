// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/TFInteractable.h"
#include "TFItemReceiver.generated.h"

USTRUCT(BlueprintType)
struct FRequiredItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ATFItem> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=1))
	int32 Count;
};

// ItemReceiver for lack of the better word atm
UCLASS()
class TF_API ATFItemReceiver : public ATFInteractable
{
	GENERATED_BODY()

public:
	void OnUse(APawn* InstigatorPawn) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnItemNotInserted();

	UFUNCTION(BlueprintNativeEvent)
	void OnItemInserted();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnCompleted();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TArray<FRequiredItem> RequiredItems;
};
