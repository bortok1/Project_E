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
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0) != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "in not null");
		//Ta linijka daje b³¹d
		//player = Cast<AEPawn>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	}

	if (player != nullptr) {
		player->OnDeath.AddUniqueDynamic(this, &APickUpSpawner::SpawnPickUp);
	}
}

// Called every frame
void APickUpSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void APickUpSpawner::SpawnPickUp()
{
	FVector location = this->GetActorLocation();
	FRotator rotation = this->GetActorRotation();
	FTransform transform = FTransform(rotation, location, FVector(1, 1, 1));
	if (pickUpToSpawn != nullptr) {
		FActorSpawnParameters SpawnParams;
		APickUp* pickUp = GetWorld()->SpawnActor<APickUp>(pickUpToSpawn, location, rotation, SpawnParams);
	}
}

