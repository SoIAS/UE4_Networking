// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TFCharacter.generated.h"

UCLASS(config=Game)
class ATFCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	ATFCharacter();

	void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	AActor* GetActorInView() const;

	/* Interactable / destructible objects should use some common base class as we got a little bit of duplicated code,
	 * since the only thing that is different when focusing the object, is UI widget
	 * but for this example I am going to leave it as it is
	 */

	/* Interactable */
	class ATFInteractable* GetInteractableInView() const;
	class ATFInteractable* GetInteractableInView(AActor* ActorInView) const;
	void UpdateInteractableFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interactable")
	void OnInteractableFocusChanged();

	UPROPERTY(BlueprintReadOnly, Category = "Interactable")
	ATFInteractable* CurrentlyFocusedInteractable;

	/* Destroyable */
	class ATFDestroyable* GetDestructibleInView() const;
	class ATFDestroyable* GetDestructibleInView(AActor* ActorInView) const;
	void UpdateDestructibleFocus();
	
	UPROPERTY(BlueprintReadOnly, Category = "Destructible")
	class ATFDestroyable* CurrentlyFocusedDestructible;

	UFUNCTION(BlueprintImplementableEvent, Category = "Destructible")
	void OnDestructibleFocusChanged();
	
	/* Usable */
	void Use();
	
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Use();

public:
	/* ITEMS / Single slot "inventory" */
	const class ATFItem* GetCurrentItem() const
	{
		return CurrentItem;
	}

	bool HasItem() const noexcept;
	bool CanPickupItem() const noexcept;
	bool PickupItem(class ATFItem* Item);
	void DestroyItem();

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DestroyItem();

protected:
	void DropItem();

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DropItem();
	
protected:
	UPROPERTY(Transient, Replicated, BlueprintReadOnly)
	class ATFItem* CurrentItem;
	
public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const
	{
		return CameraBoom;
	}
	
	FORCEINLINE class UCameraComponent* GetFollowCamera() const
	{
		return FollowCamera;
	}
};