// Fill out your copyright notice in the Description page of Project Settings.


#include "TFItemReceiver.h"

#include "TFCharacter.h"
#include "Items/TFItem.h"
#include "UnrealNetwork.h"

void ATFItemReceiver::OnUse(APawn* InstigatorPawn)
{
	if (RequiredItems.Num() == 0)
	{
		return;
	}
	
	const auto TFPawn = Cast<ATFCharacter>(InstigatorPawn);
	if (!TFPawn || !TFPawn->CurrentItem)
	{
		return;
	}

	bool bWasItemInserted = false;
	for (int i = 0; i < RequiredItems.Num(); ++i)
	{
		auto& RequiredItem = RequiredItems[i];
		if (TFPawn->CurrentItem->IsA(RequiredItem.ItemClass))
		{
			// todo
			TFPawn->CurrentItem = nullptr;
			
			bWasItemInserted = true;
			OnItemInserted();

			if (--RequiredItem.Count == 0)
			{
				RequiredItems.RemoveAt(i);
			}

			break;
		}
	}

	if (!bWasItemInserted)
	{
		OnItemNotInserted();
	}

	if (RequiredItems.Num() == 0)
	{
		OnCompleted();
	}
}

void ATFItemReceiver::OnItemInserted_Implementation()
{
	
}

void ATFItemReceiver::OnItemNotInserted_Implementation()
{
	
}

void ATFItemReceiver::OnCompleted_Implementation()
{
	
}

void ATFItemReceiver::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATFItemReceiver, RequiredItems);
}
