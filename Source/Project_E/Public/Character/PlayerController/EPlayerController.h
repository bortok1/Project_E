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
	void Shrink();
	virtual void SetupInputComponent() override;
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	virtual void BeginPlayingState() override;
	
	bool bInputPressed;
	bool bFirstInput;

	FVector Velocity;
	FVector Acceleration;
	FVector Rotation;

	FTimerHandle UnusedHandle;

	UPROPERTY()
	AEPawn* EPawn;
};
