// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "PickUpSpawner.h"
#include "MyWorld.generated.h"

class AEPawn;
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

	UFUNCTION(BlueprintCallable)
		void ListPickUps();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	FString levelName;

};