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

	const FName& GetInteractableName() const noexcept
	{
		return Name;
	}

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void OnFocusBegin();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void OnFocusEnd();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void OnUse(APawn* InstigatorPawn);
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Visuals")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	FName Name; // Todo, Maybe change to string? Do we need runtime changing?

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	FName InteractableText;
};
