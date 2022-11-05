// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPlayerController.generated.h"

class AEPawn;
/**
 * 
 */
UCLASS()
class PROJECT_E_API AEPlayerController : public APlayerController
{
	GENERATED_BODY()

	AEPlayerController();

	virtual void PlayerTick(float DeltaTime) override;
	void Grow();
	virtual void SetupInputComponent() override;
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	virtual void BeginPlayingState() override;
	
	bool bInputPressed;

	FVector Velocity;
	FVector Acceleration;
	FVector Rotation;

	UPROPERTY()
	AEPawn* EPawn;
};
