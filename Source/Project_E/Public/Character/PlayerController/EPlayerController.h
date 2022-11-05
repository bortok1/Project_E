// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_E_API AEPlayerController : public APlayerController
{
	GENERATED_BODY()

	AEPlayerController();

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	
private:
	bool bInputPressed;

	UPROPERTY()
	FVector Velocity;
};
