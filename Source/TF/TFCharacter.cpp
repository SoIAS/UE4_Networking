// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TFCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "World/TFInteractable.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"

#include "World/TFDestroyable.h"
#include "Items/TFItem.h"
#include "World/TFPickup.h"
#include "TFStaticLibrary.h"

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
	CameraBoom->SocketOffset = FVector{ 0,55,75 };

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CurrentlyFocusedInteractable = nullptr;
	CurrentlyFocusedDestructible = nullptr;
}

void ATFCharacter::Tick(const float /*DeltaSeconds*/)
{
	UpdateInteractableFocus();
	UpdateDestructibleFocus();
}

void ATFCharacter::TurnAtRate(const float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATFCharacter::LookUpAtRate(const float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATFCharacter::MoveForward(const float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATFCharacter::MoveRight(const float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ATFCharacter::SetupPlayerInputComponent(UInputComponent* const PlayerInputComponent)
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
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &ATFCharacter::DropItem);
}

AActor* ATFCharacter::GetActorInView() const
{
	if (!Controller)
	{
		return nullptr;
	}

	constexpr auto TraceLength = 1000;

	FVector CameraPosition{};
	FRotator CameraRotation{};
	Controller->GetPlayerViewPoint(CameraPosition, CameraRotation);
	
	const FVector TraceEnd = CameraPosition + CameraRotation.Vector() * TraceLength;
	const FCollisionQueryParams TraceParams{ "ActorInViewTrace", false, this };

	FHitResult Result{};
	GetWorld()->LineTraceSingleByChannel(Result, CameraPosition, TraceEnd, ECC_Visibility, TraceParams);

	return Result.GetActor();
}

ATFInteractable* ATFCharacter::GetInteractableInView() const
{
	return GetInteractableInView(GetActorInView());
}

ATFInteractable* ATFCharacter::GetInteractableInView(AActor* const ActorInView) const
{
	if (const auto Interactable = Cast<ATFInteractable>(ActorInView))
	{
		if (Interactable->CanInteractWith(this))
		{
			return Interactable;
		}
	}

	return nullptr;
}

void ATFCharacter::UpdateInteractableFocus()
{
	if (Controller && Controller->IsLocalController())
	{
		const auto Interactable = GetInteractableInView();
		bool bRefocusPending = CurrentlyFocusedInteractable == nullptr;
		bool bFocusChanged = false;
		if (CurrentlyFocusedInteractable && CurrentlyFocusedInteractable != Interactable)
		{
			CurrentlyFocusedInteractable->OnFocusEnd();
			CurrentlyFocusedInteractable->OnUsedClientCallback.Unbind();
			bRefocusPending = true;
			bFocusChanged = true;
		}

		CurrentlyFocusedInteractable = Interactable;
		if (CurrentlyFocusedInteractable && bRefocusPending)
		{
			CurrentlyFocusedInteractable->OnFocusBegin();
			CurrentlyFocusedInteractable->OnUsedClientCallback.BindUFunction(this, "OnInteractableFocusChanged");
			bFocusChanged = true;
		}

		/* We could also implement a blueprint implementable event that fires when OnFocusBegin and OnFocusEnd gets called */
		if (bFocusChanged)
		{
			OnInteractableFocusChanged();
		}
	}
}

ATFDestroyable* ATFCharacter::GetDestructibleInView() const
{
	return GetDestructibleInView(GetActorInView());
}

ATFDestroyable* ATFCharacter::GetDestructibleInView(AActor* const ActorInView) const
{
	return Cast<ATFDestroyable>(ActorInView);
}

void ATFCharacter::UpdateDestructibleFocus()
{
	// Copy paste from interactable, they should use some common base, but for this example, I am going to leave it as it is
	if (Controller && Controller->IsLocalController())
	{
		const auto Destructible = GetDestructibleInView();
		bool bRefocusPending = CurrentlyFocusedDestructible == nullptr;
		bool bFocusChanged = false;
		if (CurrentlyFocusedDestructible && CurrentlyFocusedDestructible != Destructible)
		{
			CurrentlyFocusedDestructible->OnHealthChanged.Unbind();
			bRefocusPending = true;
			bFocusChanged = true;
		}

		CurrentlyFocusedDestructible = Destructible;
		if (CurrentlyFocusedDestructible && bRefocusPending)
		{
			CurrentlyFocusedDestructible->OnHealthChanged.BindUFunction(this, "OnDestructibleFocusChanged");
			bFocusChanged = true;
		}

		if (bFocusChanged)
		{
			OnDestructibleFocusChanged();
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

	const auto ActorInView = GetActorInView();
	if (const auto Interactable = GetInteractableInView(ActorInView))
	{
		Interactable->OnUsed(this);
	}
	else if(const auto Destroyable = GetDestructibleInView(ActorInView))
	{
		Destroyable->TakeDamage(100, FDamageEvent{}, GetController(), this);
	}
}

void ATFCharacter::Server_Use_Implementation()
{
	Use();
}

bool ATFCharacter::Server_Use_Validate()
{
	return true;
}

bool ATFCharacter::HasItem() const noexcept
{
	return CurrentItem && CurrentItem->IsValidLowLevel();
}

bool ATFCharacter::CanPickupItem() const noexcept
{
	return CurrentItem == nullptr;
}

bool ATFCharacter::PickupItem(ATFItem* const Item)
{	
	if (Item && Role == ROLE_Authority)
	{
		if (CurrentItem)
		{
			return false;
		}
		
		CurrentItem = Item;
		return true;
	}

	return false;
}

void ATFCharacter::DestroyItem()
{
	if (Role < ROLE_Authority)
	{
		Server_DestroyItem();
		return;
	}

	if (CurrentItem)
	{
		CurrentItem->Destroy();
		CurrentItem = nullptr;
	}
}

void ATFCharacter::Server_DestroyItem_Implementation()
{
	DestroyItem();
}

bool ATFCharacter::Server_DestroyItem_Validate()
{
	return true;
}

void ATFCharacter::DropItem()
{
	if (Role < ROLE_Authority)
	{
		Server_DropItem();
		return;
	}

	if (CurrentItem && Controller)
	{
		// Simply dropping item at characters feet, we could also simulate physics for the item dropping
		const FVector SpawnLocation = UTFStaticLibrary::GetDropItemLocation(*this, { this });

		FActorSpawnParameters SpawnInfo{};
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ATFPickup* const DroppedItem = GetWorld()->SpawnActor<ATFPickup>(CurrentItem->GetPickupClass(), SpawnLocation, FRotator::ZeroRotator, SpawnInfo);

		if (DroppedItem)
		{
			DestroyItem();
		}
	}
}

void ATFCharacter::Server_DropItem_Implementation()
{
	DropItem();
}

bool ATFCharacter::Server_DropItem_Validate()
{
	return true;
}

void ATFCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ATFCharacter, CurrentItem, COND_OwnerOnly);
}
