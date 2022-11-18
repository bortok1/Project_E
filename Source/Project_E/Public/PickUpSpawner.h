// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/EPawn.h"
#include "PickUp.h"
#include "PickUpSpawner.generated.h"

class UPostInitializeComponent;

UCLASS()
class PROJECT_E_API APickUpSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpSpawner();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<APickUp> pickUpToSpawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AEPawn* player;
	void SpawnPickUp();

};
