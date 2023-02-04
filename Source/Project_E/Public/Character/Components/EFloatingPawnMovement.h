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

	UPROPERTY()
	AEPawn* Owner;
	
	virtual void BeginPlay() override;

public:
	void Move(float DeltaTime, bool bMoveInputPressed);

	// To jump subtract from this value 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float JumpVelocity;
	
private:
	UFUNCTION()
	void StopMovement();
	
	void Gravity(float DeltaTime);
	bool isAirTime;
	bool bStopAfterReset;
	float GravityForce;
	float SpeedDumpWhileAirTime;
	FVector LastCursorLocation;
	
	FVector GetVectorTowardsCursor() const;
	FVector2D GetMousePosition() const;

	float MoveSpeed;
};
