// Copyright Bean Bin, All Rights Reserved.


#include "Character/Components/EFloatingPawnMovement.h"
#include "Character/EPawn.h"
#include "Character/Components/SizeManagerComponent.h"
#include "Character/PlayerController/EPlayerController.h"
#include "GameFramework/PlayerController.h"

UEFloatingPawnMovement::UEFloatingPawnMovement()
{
	GravityForce = 478.f;
	MoveSpeed = 1.f;
	SpeedDumpWhileAirTime = 0.5f;
	
	isAirTime = true;
	bStopAfterReset = true;
}

void UEFloatingPawnMovement::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AEPawn>(GetOwner());
	Owner->OnDeath.AddUniqueDynamic(this, &UEFloatingPawnMovement::StopMovement);

	FVector2D ScreenResolution;
	GEngine->GameViewport->GetViewportSize(ScreenResolution);
	LastCursorLocation = FVector((ScreenResolution * 0.5f), 0);
}

void UEFloatingPawnMovement::Move(float DeltaTime, bool bMoveInputPressed = false)
{
	Gravity(DeltaTime);

	if(bMoveInputPressed && !isAirTime)
	{
		LastCursorLocation = GetVectorTowardsCursor();
		Owner->AddMovementInput(LastCursorLocation, MoveSpeed/Owner->SizeComponent->GetMass());
		Owner->CharacterMesh->SetRelativeRotation(LastCursorLocation.Rotation());
		bStopAfterReset = false;
	}
	else if(isAirTime && !bStopAfterReset){
		Owner->AddMovementInput(LastCursorLocation, MoveSpeed/Owner->SizeComponent->GetMass());
		Owner->CharacterMesh->SetRelativeRotation(GetVectorTowardsCursor().Rotation());
	}
}

void UEFloatingPawnMovement::StopMovement()
{
	bStopAfterReset = true;
	StopMovementImmediately();
}

void UEFloatingPawnMovement::Gravity(float DeltaTime)
{
	JumpVelocity += GravityForce * DeltaTime;

	FVector TeleportLocation = Owner->GetActorLocation();
	TeleportLocation.Z -= JumpVelocity * DeltaTime + 0.5 * GravityForce * DeltaTime * DeltaTime;

	if (Owner->TeleportTo(TeleportLocation, Owner->GetActorRotation(), true)) {
		Owner->TeleportTo(TeleportLocation, Owner->GetActorRotation(), false, true);
		
		isAirTime = true;
		LastCursorLocation -= LastCursorLocation * SpeedDumpWhileAirTime * DeltaTime;
	}
	else {
		JumpVelocity = GravityForce * DeltaTime;
		isAirTime = false;
	}
}

FVector UEFloatingPawnMovement::GetVectorTowardsCursor() const
{
	// Take half of screen resolution
	FVector2D HalfResolution = FVector2D::Zero();
	if ( GEngine && GEngine->GameViewport )
	{
		GEngine->GameViewport->GetViewportSize(HalfResolution);
		HalfResolution *= 0.5f;
	}

	// Direction from screen center (player) to mouse cursor
	//Windows settings
	//return FVector((GetMousePosition() - HalfResolution).GetSafeNormal(),0);

	//Mobile settings
	//return FVector(GetThumbstickPosition(), 0);
}

//For Windows settings
FVector2D UEFloatingPawnMovement::GetMousePosition() const
{
	FVector2D Mouse = FVector2d::Zero();
	Owner->EPlayerController->GetMousePosition(Mouse.X, Mouse.Y);
	return Mouse;
}

//For mobile settings
FVector2D UEFloatingPawnMovement::GetThumbstickPosition() const
{
	FVector2D Thumbstick = Owner->GetJoystickAxis();
	return Thumbstick;
}