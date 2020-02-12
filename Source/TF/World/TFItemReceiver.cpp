// Fill out your copyright notice in the Description page of Project Settings.


#include "TFItemReceiver.h"

#include "TFCharacter.h"
#include "Items/TFItem.h"
#include "UnrealNetwork.h"

ATFItemReceiver::ATFItemReceiver()
{
	bIsCompleted = false;
}

void ATFItemReceiver::BeginPlay()
{
	if (Role == ROLE_Authority)
	{
		CheckForCompleted();
	}
}

void ATFItemReceiver::OnUsed(APawn* InstigatorPawn)
{
	if (IsCompleted())
	{
		return;
	}
	
	const auto TFPawn = Cast<ATFCharacter>(InstigatorPawn);
	if (!TFPawn || !TFPawn->CurrentItem)
	{
		return;
	}

	for (int i = 0; i < RequiredItems.Num(); ++i)
	{
		auto& RequiredItem = RequiredItems[i];
		if (TFPawn->CurrentItem->IsA(RequiredItem.ItemClass))
		{
			TFPawn->DestroyItem();
			
			if (--RequiredItem.Count == 0)
			{
				RequiredItems.RemoveAt(i);
			}

			break;
		}
	}

	CheckForCompleted();

	Super::OnUsed(InstigatorPawn);
}

void ATFItemReceiver::CheckForCompleted()
{
	if (RequiredItems.Num() == 0 && !IsCompleted())
	{
		bIsCompleted = true;
		OnRep_IsCompleted();
	}
}

void ATFItemReceiver::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATFItemReceiver, RequiredItems);
	DOREPLIFETIME(ATFItemReceiver, bIsCompleted);
}
