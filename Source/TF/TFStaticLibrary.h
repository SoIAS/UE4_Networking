// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TFStaticLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TF_API UTFStaticLibrary : public UObject
{
	GENERATED_BODY()

public:
	static FVector GetDropItemLocation(const AActor& Instigator, const TArray<class AActor*>& IgnoredActors);
	
};
