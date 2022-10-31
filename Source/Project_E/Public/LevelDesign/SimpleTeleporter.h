// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SimpleTeleporter.generated.h"

UCLASS()
class PROJECT_E_API ASimpleTeleporter : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FVector NewLocation = FVector3d(350, 2450, 50);
	
	// Sets default values for this actor's properties
	ASimpleTeleporter();
	
protected:
	virtual void Tick(float DeltaSeconds) override;
private:
	UPROPERTY()
	UBoxComponent* CollisionMesh;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
