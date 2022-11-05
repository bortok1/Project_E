// Copyright Bean Bin, All Rights Reserved.


#include "Character/PlayerController/EPlayerController.h"

#include <algorithm>

#include "Math/UnitConversion.h"

AEPlayerController::AEPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	bInputPressed = false;
	Velocity = FVector::Zero();
}

void AEPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if(APawn* const MyPawn = GetPawn())
	{
		if(bInputPressed)
		{
			// Look for the touch location
			FVector HitLocation = FVector::ZeroVector;
			FHitResult Hit;

			GetHitResultUnderCursor(ECC_Visibility, true, Hit);

			HitLocation = Hit.Location;
		
			// Direct the Pawn towards that location
			FVector PawnLocation = MyPawn->GetActorLocation();
			FVector WorldDirection = (HitLocation - PawnLocation).GetSafeNormal();
			WorldDirection.Z = 0.f;
			Velocity += WorldDirection * DeltaTime * 25.f;
		}
		if(!Velocity.IsZero())
		{
			MyPawn->TeleportTo(MyPawn->GetActorLocation() + Velocity, Velocity.Rotation());
			Velocity -= Velocity * DeltaTime;
		}
	}
}

void AEPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AEPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AEPlayerController::OnSetDestinationReleased);
}

void AEPlayerController::OnSetDestinationPressed()
{
	if(GetPawn())
	{
		// We flag that the input is being pressed
		bInputPressed = true;
		// Just in case the character was moving because of a previous short press we stop it
		StopMovement();
	}
}

void AEPlayerController::OnSetDestinationReleased()
{
	if(GetPawn())
	{
		// Player is no longer pressing the input
		bInputPressed = false;
	}
}
