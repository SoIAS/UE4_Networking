// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TFGameMode.h"
#include "TFCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ATFGameMode::ATFGameMode()
{
	CurrentPlayerStart = 0;
}

// Simply iterate over all player starts and choose a next one, so we don't get two player at the same start even if other starts are not used
AActor* ATFGameMode::ChoosePlayerStart_Implementation(AController* const Player)
{	
	if (PlayerStarts.Num() == 0)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

		// if still empty
		if (PlayerStarts.Num() == 0)
		{
			return Super::ChoosePlayerStart_Implementation(Player);
		}
	}

	if (CurrentPlayerStart >= PlayerStarts.Num())
	{
		CurrentPlayerStart = 0;
	}

	return PlayerStarts[CurrentPlayerStart++];
}
