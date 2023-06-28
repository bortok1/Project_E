// Copyright Bean Bin, All Rights Reserved.


#include "LevelDesign/PickUpSpawner.h"
#include "LevelDesign/PickUp.h"

void APickUpSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnPickUp();
}

APickUp* APickUpSpawner::SpawnPickUp()
{
	APickUp* pickUp = nullptr;
	
	if (pickUpToSpawn != nullptr) {
		const FVector location = this->GetActorLocation();
		const FRotator rotation = this->GetActorRotation();
		const FActorSpawnParameters SpawnParams;
		pickUp = GetWorld()->SpawnActor<APickUp>(pickUpToSpawn, location, rotation, SpawnParams);
	}
	return pickUp;
}
