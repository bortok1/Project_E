// Copyright Bean Bin, All Rights Reserved.

#include "PickUpSpawner.h"
#include "PickUp.h"
#include <Kismet/GameplayStatics.h>
#include "Engine/World.h"
#include "Engine/Engine.h"

// Sets default values
APickUpSpawner::APickUpSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickUpSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnPickUp();
}

// Called every frame
void APickUpSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


APickUp* APickUpSpawner::SpawnPickUp()
{
	APickUp* pickUp = nullptr;
	FVector location = this->GetActorLocation();
	FRotator rotation = this->GetActorRotation();
	FTransform transform = FTransform(rotation, location, FVector(1, 1, 1));
	if (pickUpToSpawn != nullptr) {
		FActorSpawnParameters SpawnParams;
		pickUp = GetWorld()->SpawnActor<APickUp>(pickUpToSpawn, location, rotation, SpawnParams);
	}
	return pickUp;
}

