// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "PickUp.generated.h"

UCLASS()
class PROJECT_E_API APickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void DestroyPU();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
