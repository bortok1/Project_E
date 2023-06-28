// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "PickUpSpawner.generated.h"

class APickUp;

UCLASS()
class PROJECT_E_API APickUpSpawner : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<APickUp> pickUpToSpawn;

public:	
	APickUp* SpawnPickUp();
};
