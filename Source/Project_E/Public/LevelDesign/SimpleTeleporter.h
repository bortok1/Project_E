// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "SimpleTeleporter.generated.h"

class UBoxComponent;
UCLASS()
class PROJECT_E_API ASimpleTeleporter : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FVector NewLocation = FVector3d(350, 2450, 100);
	
	// Sets default values for this actor's properties
	ASimpleTeleporter();

private:
	UPROPERTY()
	UBoxComponent* CollisionMesh;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
