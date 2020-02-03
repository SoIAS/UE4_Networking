// Fill out your copyright notice in the Description page of Project Settings.


#include "TFLightSwitch.h"
#include "UnrealNetwork.h"
#include "Engine/Light.h"
#include "Components/LightComponent.h"

void ATFLightSwitch::BeginPlay()
{
	OnRep_bIsLit();
}

void ATFLightSwitch::OnUse(APawn* const /*InstigatorPawn*/)
{
	bIsLit = !bIsLit;
	OnRep_bIsLit();
}

void ATFLightSwitch::OnRep_bIsLit()
{
	for (auto& LightActor : LightActors)
	{
		if (const auto LightComponent = LightActor->GetLightComponent())
		{
			LightComponent->SetVisibility(bIsLit);
		}
	}
	
	OnLightToggled();
}

void ATFLightSwitch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATFLightSwitch, bIsLit);
}
