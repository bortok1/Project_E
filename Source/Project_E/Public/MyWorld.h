// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>

#include "PickUp.h"
#include "PickUpSpawner.h"
#include "Character/EPawn.h"
#include "MyWorld.generated.h"

UCLASS()
class PROJECT_E_API AMyWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWorld();

	AEPawn* player;
	std::vector<APickUp*> pickUps;
	std::vector<APickUpSpawner*> pickUpSpawners;
	
	UFUNCTION()
	void Clear();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};