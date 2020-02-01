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

