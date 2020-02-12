// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TFCharacter.generated.h"

UCLASS(config=Game)
class ATFCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
public:
	ATFCharacter();

	void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	
protected:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class ATFInteractable* GetInteractableInView() const;
	void UpdateInteractableFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interactable")
	void OnInteractableFocusChanged();
	// todo, move the functions to correct positions

	UPROPERTY(BlueprintReadOnly, Category = "Interactable")
	ATFInteractable* CurrentlyFocused;
	
	void Use();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Use();

protected:
public: // todo
	UPROPERTY(Transient, Replicated, BlueprintReadOnly)
	class ATFItem* CurrentItem;

	class ATFDestroyable* GetDestroyableInView() const; // tmp
	
public:
	bool CanPickupItem() const noexcept;
	bool PickupItem(ATFItem* Item);
	

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Destructible")
	ATFDestroyable* CurrentlyFocusedDestructible;

	void UpdateDestructibleFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "Destructible")
	void OnDestructibleFocusChanged();

public:
	void DestroyItem();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DestroyItem();

protected:
	void DropItem();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DropItem();
	
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