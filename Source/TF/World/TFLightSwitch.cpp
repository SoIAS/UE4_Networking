// Fill out your copyright notice in the Description page of Project Settings.


#include "TFLightSwitch.h"
#include "Engine/Light.h"
#include "Components/LightComponent.h"

void ATFLightSwitch::OnRep_IsToggled_Implementation()
{
	Super::OnRep_IsToggled_Implementation();
	
	for (auto& LightActor : LightActors)
	{
		if (const auto LightComponent = LightActor->GetLightComponent())
		{
			LightComponent->SetVisibility(bIsToggled);
		}
	}
}