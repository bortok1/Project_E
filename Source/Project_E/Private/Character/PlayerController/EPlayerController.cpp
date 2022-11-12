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
	bResetTimer = true;
	Velocity = FVector::Zero();
	Acceleration = FVector::Zero();
	Rotation = FVector::Zero();

	GrowTimer = 0;
}

void AEPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AEPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AEPlayerController::OnSetDestinationReleased);
	
	InputComponent->BindAction("Grow", IE_Pressed, this, &AEPlayerController::Grow);
	InputComponent->BindAction("Shrink", IE_Pressed, this, &AEPlayerController::Shrink);
}

void AEPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	EPawn = Cast<AEPawn>(GetPawn());
	
	FTimerHandle Handle;	
	FTimerDelegate Delegate;
	float DeltaTime = 0.013f;
	Delegate.BindUObject(this, &AEPlayerController::MoveTick, DeltaTime);

	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
		if (World)
		{
			World->GetTimerManager().SetTimer(Handle, Delegate, DeltaTime, true);
		}
	}
}

void AEPlayerController::MoveTick(float DeltaTime)
{
	if(EPawn)
	{
		if(bInputPressed)
		{
			if (bFirstInput && bResetTimer) {
				bResetTimer = false;
				EPawn->ResetTimer();
			}
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
			if(!EPawn->TeleportTo(EPawn->GetActorLocation() + Velocity, Rotation.Rotation()))
				Die();
			else
			{
				Velocity *= EPawn->GetFriction();
				GetWorldTimerManager().ClearTimer(GrowTimeHandle);
			}
		}
		else if(bFirstInput && Velocity.Length() < 10.f / EPawn->GetMass()) {
			if (bFirstInput) {
				GetWorld()->GetTimerManager().SetTimer(
					GrowTimeHandle, this, &AEPlayerController::Grow, EPawn->GetGrowSpeed(), true, EPawn->GetGrowSpeed());
			}
		}

		MoveCamera();
	}
}

void AEPlayerController::Die()
{
	Acceleration = FVector::Zero();
	Velocity = FVector::Zero();
	Rotation = FVector::Zero();
	bInputPressed = false; 
	bFirstInput = false;
	bResetTimer = true;
	EPawn->StopTimer();
	EPawn->OnHit(GetSpawnLocation());
	GetWorldTimerManager().ClearTimer(GrowTimeHandle);
}

void AEPlayerController::Grow()
{
	GrowTimer = 0;
	if (EPawn->GrowBox() && !EPawn->TeleportTo(EPawn->GetActorLocation() + Velocity + FVector(0.001f, 0.f,0.f), Rotation.Rotation(), true))
		Die();
}

void AEPlayerController::Shrink()
{
	EPawn->ShrinkBox();
}

void AEPlayerController::OnSetDestinationPressed()
{
	bInputPressed = true;
	bFirstInput = true;
}

void AEPlayerController::OnSetDestinationReleased()
{
	bInputPressed = false;
}

void AEPlayerController::MoveCamera()
{
	FVector EPawnLocation = EPawn->GetActorLocation();
	FVector PlaneOrigin(0.0f, 0.0f, EPawnLocation.Z);
	FVector CursorLocation, CursorRotation;
	if (DeprojectMousePositionToWorld(CursorLocation, CursorRotation))
	{
		CursorLocation = FMath::LinePlaneIntersection(
			CursorLocation,
			CursorLocation + CursorRotation,
			PlaneOrigin,
			FVector::UpVector);
	}
	else
	{
		CursorLocation = EPawnLocation;
	}
	EPawnLocation = FVector(EPawnLocation.X, EPawnLocation.Y, 0);
	CursorLocation = FVector(CursorLocation.X, CursorLocation.Y, 0);
	FVector Normalized = (CursorLocation - EPawnLocation).GetSafeNormal();
	float Distance = FVector::Distance(EPawnLocation, CursorLocation);
	float BoomZ = EPawn->CameraBoom->GetComponentToWorld().GetLocation().Z;
	FVector NewCameraLocation = Normalized * FMath::Clamp(Distance * 0.5f, 0.0, EPawn->GetCameraMaxOffset()) + EPawnLocation;
	NewCameraLocation.Z = BoomZ;
	EPawn->CameraBoom->SetWorldLocation(NewCameraLocation);
}



