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
	ATFItemReceiver();
	
protected:
	void BeginPlay() override;

public:
	void OnUse(APawn* InstigatorPawn) override;

	const TArray<FRequiredItem>& GetRequiredItems() const noexcept
	{
		return RequiredItems;
	}
	
	bool IsCompleted() const noexcept
	{
		return bIsCompleted;
	}

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
	TArray<FRequiredItem> RequiredItems;

	UPROPERTY(ReplicatedUsing = OnRep_IsCompleted, BlueprintReadOnly)
	bool bIsCompleted;

	UFUNCTION(BlueprintImplementableEvent)
	void OnRep_IsCompleted();

	void CheckForCompleted();
};
