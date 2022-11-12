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
	virtual void SetupInputComponent() override;
	virtual void BeginPlayingState() override;
	void MoveTick(float DeltaTime);
	void Die();
	void Grow();
	void Shrink();
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	void MoveCamera();
	
	bool bInputPressed;
	bool bFirstInput;
	bool bResetTimer;

	FVector Velocity;
	FVector Acceleration;
	FVector Rotation;
	
	FTimerHandle GrowTimeHandle;

	int8 GrowTimer;
	
	UPROPERTY()
	AEPawn* EPawn;
};
