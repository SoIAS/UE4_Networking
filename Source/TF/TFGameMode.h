// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TFGameMode.generated.h"

UCLASS(minimalapi)
class ATFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATFGameMode();
	
protected:
	void BeginPlay() override;

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UPROPERTY()
	TArray<AActor*> PlayerStarts;

	int CurrentPlayerStart;
};



