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
	
	SetReplicates(true);
}

void ATFInteractable::OnFocusBegin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Object focused"));
}

void ATFInteractable::OnFocusEnd()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Object unfocused"));
}

void ATFInteractable::OnUse(APawn* const /*InstigatorPawn*/)
{
	LowLevelFatalError(TEXT("Pure virtual function not implemented"));
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