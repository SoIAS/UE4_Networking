// Fill out your copyright notice in the Description page of Project Settings.

#include "TFInteractable.h"
#include "Engine/Engine.h"

#include "Components/StaticMeshComponent.h"

ATFInteractable::ATFInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(GetStaticMeshComponent());

	Name = "Default Name";
	ActionText = "";

	MaxInteractionDistance = 200;
	
	SetReplicates(true);
}

void ATFInteractable::OnFocusBegin()
{
	// In case we want to add custom stencil/outline
}

void ATFInteractable::OnFocusEnd()
{
}

void ATFInteractable::OnUsed(APawn* const /*InstigatorPawn*/)
{
	NetMulticast_OnUsed();
}

bool ATFInteractable::CanInteractWith_Implementation(const APawn* const Pawn)
{
	return GetDistanceTo(Pawn) <= MaxInteractionDistance;
}

FString ATFInteractable::GetInteractableTooltipText_Implementation()
{
	FString TooltipText{};
	if (ActionText.GetStringLength() != 0)
	{
		TooltipText = ActionText.ToString() + ' ';
	}

	TooltipText += Name.ToString();
	return TooltipText;
}

void ATFInteractable::NetMulticast_OnUsed_Implementation()
{
	Blueprint_NetMulticast_OnUsed();
}