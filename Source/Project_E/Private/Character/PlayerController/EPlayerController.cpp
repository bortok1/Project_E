// Copyright Bean Bin, All Rights Reserved.


#include "Character/PlayerController/EPlayerController.h"
#include "InputMappingContext.h"

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

	//Mobile settings
	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis2D;
	PawnMappingContext->MapKey(MoveAction, EKeys::Gamepad_Right2D);
}
