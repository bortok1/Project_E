// Copyright Bean Bin, All Rights Reserved.


#include "Character/PlayerController/EPlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Character/EPawn.h"
#include "Math/UnitConversion.h"

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

void AEPlayerController::Win()
{
	EPawn->WriteScoreTimer();
}

void AEPlayerController::MoveCamera()
{
	// FVector EPawnLocation = EPawn->GetActorLocation();
	// FVector PlaneOrigin(0.0f, 0.0f, EPawnLocation.Z);
	// FVector CursorLocation2, CursorRotation;
	// if (DeprojectMousePositionToWorld(CursorLocation2, CursorRotation))
	// {
	// 	CursorLocation2 = FMath::LinePlaneIntersection(
	// 		CursorLocation2,
	// 		CursorLocation2 + CursorRotation,
	// 		PlaneOrigin,
	// 		FVector::UpVector);
	// }
	// else
	// {
	// 	CursorLocation2 = EPawnLocation;
	// }
	// EPawnLocation = FVector(EPawnLocation.X, EPawnLocation.Y, 0);
	// CursorLocation2 = FVector(CursorLocation2.X, CursorLocation2.Y, 0);
	// FVector Normalized = (CursorLocation2 - EPawnLocation).GetSafeNormal();
	// float Distance = FVector::Distance(EPawnLocation, CursorLocation2);
	// float BoomZ = EPawn->CameraBoom->GetComponentToWorld().GetLocation().Z;
	// FVector NewCameraLocation = Normalized * FMath::Log2(Distance/EPawn->GetCameraMovementDivider() + 1) * EPawn->GetCameraMovementMultiplier() + EPawnLocation;
	// NewCameraLocation.Z = BoomZ;
	// EPawn->CameraBoom->SetWorldLocation(NewCameraLocation);
}



