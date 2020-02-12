// Fill out your copyright notice in the Description page of Project Settings.

#include "TFLightSwitch.h"
#include "Engine/Light.h"
#include "Components/LightComponent.h"

void ATFLightSwitch::OnRep_IsToggled_Implementation()
{
	Super::OnRep_IsToggled_Implementation();

	// Obviously we can do bunch of checks for valid actors/components and log the results so we can find bugs etc.
	// But since this is a networking test, I am going to leave it as it is for readability
	for (auto& LightActor : LightActors)
	{
		if (const auto LightComponent = LightActor->GetLightComponent())
		{
			LightComponent->SetVisibility(bIsToggled);
		}
	}

	for (const auto& LightComponentReference : LightComponents)
	{
		const auto ComponentOwner = LightComponentReference.OtherActor ? LightComponentReference.OtherActor : this;
		if (const auto LightComponent = Cast<ULightComponent>(LightComponentReference.GetComponent(ComponentOwner)))
		{
			LightComponent->SetVisibility(bIsToggled);
		}
	}
}