// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "GameFramework/FloatingPawnMovement.h"
#include "EFloatingPawnMovement.generated.h"

class AEPawn;

UCLASS()
class PROJECT_E_API UEFloatingPawnMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()

	UEFloatingPawnMovement();
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	AEPawn* Owner;

	UFUNCTION()
	void StopImmediately();
	
	void Gravity(float DeltaTime) const;
	
	UPROPERTY(EditAnywhere, Category="Movement")
	float GravityForce;
	
public:
	static FVector GetVectorTowardsCursor(FVector2D CursorLocation);

	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveScale;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AngularDumping;
};
