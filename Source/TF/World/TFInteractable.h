// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFInteractable.generated.h"

UCLASS()
class TF_API ATFInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	ATFInteractable();

	UFUNCTION(BlueprintCallable, Category = "Visuals")
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const
	{
		return StaticMeshComponent;
	}

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void OnFocusBegin();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void OnFocusEnd();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void OnUsed(APawn* UsedBy);

	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Visuals")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Name")
	FName Name;

};
