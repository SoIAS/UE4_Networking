#include "TFItem.h"

ATFItem::ATFItem()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetReplicates(true);
}

void ATFItem::BeginPlay()
{
	Super::BeginPlay();
}