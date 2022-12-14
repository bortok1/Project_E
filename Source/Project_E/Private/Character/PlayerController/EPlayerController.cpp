// Copyright Bean Bin, All Rights Reserved.


#include "Character/PlayerController/EPlayerController.h"
#include "InputMappingContext.h"
#include "Character/EPawn.h"

AEPlayerController::AEPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AEPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Create these objects here and not in constructot because we only need them on the client.
	PawnMappingContext = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Boolean;
	PawnMappingContext->MapKey(MoveAction, EKeys::LeftMouseButton);
}

void AEPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	EPawn = Cast<AEPawn>(GetPawn());
}




