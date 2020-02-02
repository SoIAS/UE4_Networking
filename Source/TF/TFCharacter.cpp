// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TFCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"


#include "World/TFInteractable.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"

#include "World/TFDestroyable.h"


ATFCharacter::ATFCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;	
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// todo, move to begin play?
	CurrentlyFocused = nullptr;
}

void ATFCharacter::Tick(float DeltaSeconds)
{
	UpdateInteractableFocus();
}

void ATFCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATFCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATFCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATFCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATFCharacter::LookUpAtRate);
	
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ATFCharacter::Use);
}

ATFInteractable* ATFCharacter::GetInteractableInView() const
{
	if (!Controller)
	{
		return nullptr;
	}

	constexpr auto TraceLength = 1000;

	FVector CameraPosition{};
	FRotator CameraRotation{};
	Controller->GetPlayerViewPoint(CameraPosition, CameraRotation);
;
	const FVector TraceEnd = CameraPosition + CameraRotation.Vector() * TraceLength;
	const FCollisionQueryParams TraceParams{ "InteractableTrace", false, this };

	FHitResult Result{};
	GetWorld()->LineTraceSingleByChannel(Result, CameraPosition, TraceEnd, ECC_Visibility, TraceParams);

	return Cast<ATFInteractable>(Result.GetActor());
}

void ATFCharacter::UpdateInteractableFocus()
{
	if (Controller && Controller->IsLocalController())
	{
		const auto Interactable = GetInteractableInView();
		bool bRefocusPending{ CurrentlyFocused == nullptr };
		bool bFocusChanged{ false };
		if (CurrentlyFocused && CurrentlyFocused != Interactable)
		{
			CurrentlyFocused->OnFocusEnd();
			bRefocusPending = true;
			bFocusChanged = true;
		}

		CurrentlyFocused = Interactable;
		if (CurrentlyFocused && bRefocusPending)
		{
			CurrentlyFocused->OnFocusBegin();
			bFocusChanged = true;
		}

		/* We could also implement a blueprint implementable event that fires when OnFocusBegin and OnFocusEnd gets called */
		if (bFocusChanged)
		{
			OnInteractableFocusChanged();
		}
	}
}

void ATFCharacter::Use()
{
	if (Role < ROLE_Authority)
	{
		Server_Use();
		return;
	}

	if (const auto Interactable = GetInteractableInView())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("USED CALLED"));
		Interactable->OnUse(this);
	}
	else if(const auto Destroyable = GetDestroyableInView())
	{
		Destroyable->TakeDamage(100, FDamageEvent{}, GetController(), this);
	}
}

ATFDestroyable* ATFCharacter::GetDestroyableInView() const
{
	// simple copy paste from get interactable, todo
	if (!Controller)
	{
		return nullptr;
	}

	constexpr auto TraceLength = 1000;

	FVector CameraPosition{};
	FRotator CameraRotation{};
	Controller->GetPlayerViewPoint(CameraPosition, CameraRotation);
	;
	const FVector TraceEnd = CameraPosition + CameraRotation.Vector() * TraceLength;
	const FCollisionQueryParams TraceParams{ "InteractableTrace", false, this };

	FHitResult Result{};
	GetWorld()->LineTraceSingleByChannel(Result, CameraPosition, TraceEnd, ECC_Visibility, TraceParams);

	return Cast<ATFDestroyable>(Result.GetActor());
}

bool ATFCharacter::PickupItem(ATFItem* Item)
{	
	if (Item && Role == ROLE_Authority)
	{
		if (CurrentItem)
		{
			return false;
		}
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ItemSet"));

		CurrentItem = Item;
		return true;
	}

	return false;
}


void ATFCharacter::Server_Use_Implementation()
{
	Use();
}

bool ATFCharacter::Server_Use_Validate()
{
	return true;
}


void ATFCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATFCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATFCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATFCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ATFCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ATFCharacter, CurrentItem, COND_OwnerOnly);
}
