// Copyright Bean Bin, All Rights Reserved.


#include "Character/PlayerController/EPlayerController.h"

#include "Character/EPawn.h"
#include "TimerManager.h"
#include "Math/UnitConversion.h"

AEPlayerController::AEPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	bInputPressed = false;
	bFirstInput = false;
	Velocity = FVector::Zero();
	Acceleration = FVector::Zero();
	Rotation = FVector::Zero();

}
void AEPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	EPawn = Cast<AEPawn>(GetPawn());
}

void AEPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if(EPawn)
	{
		if(bInputPressed)
		{
			FHitResult Hit;
			GetHitResultUnderCursor(ECC_Visibility, true, Hit);
			FVector HitLocation = Hit.Location;
			FVector ActorLocation = EPawn->GetActorLocation();
			
			FVector WorldDirection = (HitLocation - ActorLocation).GetSafeNormal();
			WorldDirection.Z = 0.;
			
			Acceleration = (WorldDirection * EPawn->GetSpeed() / DeltaTime) / EPawn->GetMass();
			Velocity += Acceleration;
			Rotation = (Rotation * (EPawn->GetAngularDumping() - 1) + Acceleration) / EPawn->GetAngularDumping();
		}
		if(Velocity.Length() > 0.01f)
		{
			GetWorldTimerManager().ClearTimer(UnusedHandle);
			if(!EPawn->TeleportTo(EPawn->GetActorLocation() + Velocity, Rotation.Rotation()))
			{
				Acceleration = FVector::Zero();
				Velocity = FVector::Zero();
				Rotation = FVector::Zero();
				EPawn->OnHit(nullptr, nullptr, nullptr, FVector::Zero(), FHitResult());
			}
			Velocity *= EPawn->GetFriction();
		}
		else if(Velocity.Length() != 0) {
			if (bFirstInput) {
				GetWorld()->GetTimerManager().SetTimer(
					UnusedHandle, this, &AEPlayerController::Grow, EPawn->GetGrowSpeed(), true, EPawn->GetGrowSpeed());
			}
			Velocity = FVector::Zero();
		}
	}
}

void AEPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AEPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AEPlayerController::OnSetDestinationReleased);
	
	InputComponent->BindAction("Grow", IE_Pressed, this, &AEPlayerController::Grow);
	InputComponent->BindAction("Shrink", IE_Pressed, this, &AEPlayerController::Shrink);
}

void AEPlayerController::Grow()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Grow");
	EPawn->GrowBox();
}

void AEPlayerController::Shrink()
{
	EPawn->ShrinkBox();
}

void AEPlayerController::OnSetDestinationPressed()
{
	// We flag that the input is being pressed
	bInputPressed = true;
	bFirstInput = true;
	// Just in case the character was moving because of a previous short press we stop it
	StopMovement();
}

void AEPlayerController::OnSetDestinationReleased()
{
	// Player is no longer pressing the input
	bInputPressed = false;
}

