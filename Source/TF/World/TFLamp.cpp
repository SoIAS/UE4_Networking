// Fill out your copyright notice in the Description page of Project Settings.


#include "TFLamp.h"
#include "Components/PointLightComponent.h"
#include "UnrealNetwork.h"

ATFLamp::ATFLamp()
{
	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>("Light");
}

void ATFLamp::BeginPlay()
{
	if (Role == ROLE_Authority)
	{
		bIsLit = PointLightComponent->IsVisible();
	}
}

void ATFLamp::OnUse(APawn* const /*InstigatorPawn*/)
{
	bIsLit = !bIsLit;

	// For someone who is playing on listening server
	ToggleLight();
}

void ATFLamp::OnRep_IsLit()
{
	ToggleLight();
}

void ATFLamp::ToggleLight() const
{
	if (PointLightComponent)
	{
		PointLightComponent->SetVisibility(bIsLit);
	}
}

void ATFLamp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATFLamp, bIsLit);
}
