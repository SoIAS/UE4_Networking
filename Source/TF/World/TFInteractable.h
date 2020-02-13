// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFInteractable.generated.h"


DECLARE_DELEGATE(OnUsedDelegate);

/* It would be better if interactable was an interface (that pickup and other stuff would implement), but for now I will leave it as is */
UCLASS(Abstract)
class TF_API ATFInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	ATFInteractable();

	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const
	{
		return StaticMeshComponent;
	}

	FORCEINLINE const FName& GetInteractableName() const noexcept
	{
		return Name;
	}

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void OnFocusBegin();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void OnFocusEnd();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	virtual void OnUsed(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	bool CanInteractWith(const APawn* Pawn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Display")
	FString GetInteractableTooltipText();

	OnUsedDelegate OnUsedClientCallback;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Visuals")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	FName ActionText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float MaxInteractionDistance;
	
private:
	UFUNCTION(Reliable, NetMulticast)
	void NetMulticast_OnUsed();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void Blueprint_NetMulticast_OnUsed();
};