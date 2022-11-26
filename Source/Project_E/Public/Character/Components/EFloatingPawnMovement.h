// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerController/EPlayerController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EFloatingPawnMovement.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_E_API UEFloatingPawnMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	AEPawn* Owner;

public:
	static FVector GetVectorTowardsCursor(FVector2D CursorLocation);

	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveScale;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AngularDumping;
};
