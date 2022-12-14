// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "EPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

class AEPawn;
/**
 * 
 */
UCLASS()
class PROJECT_E_API AEPlayerController : public APlayerController
{
	GENERATED_BODY()

	AEPlayerController();

	/** Setup input actions and context mappings for player */
	virtual void SetupInputComponent() override;

public:
	/** Mapping context used for pawn contol. */
	UPROPERTY()
	class UInputMappingContext* PawnMappingContext;
	
	UPROPERTY()
	class UInputAction* MoveAction;

private:
	virtual void BeginPlayingState() override;
	
	UPROPERTY()
	AEPawn* EPawn;
};
