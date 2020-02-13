// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFItem.generated.h"

UCLASS(Abstract)
class TF_API ATFItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ATFItem();

	const TSubclassOf<class ATFPickup>& GetPickupClass() const noexcept
	{
		return PickupClass;
	}
	
protected:
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	TSubclassOf<class ATFPickup> PickupClass;
};