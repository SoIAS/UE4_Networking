// Fill out your copyright notice in the Description page of Project Settings.

#include "TFInteractable.h"
#include <Runtime/Engine/Classes/Engine/Engine.h>


ATFInteractable::ATFInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	Name = "Default Name";

	SetReplicates(true);
}

void ATFInteractable::OnFocusBegin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Object focused"));
}

void ATFInteractable::OnFocusEnd()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Object unfocused"));
}

void ATFInteractable::OnUsed(APawn* const /*UsedBy*/)
{
	LowLevelFatalError(TEXT("Pure virtual function not implemented"));
}
