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
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Velocity.ToString());
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
			if(!EPawn->TeleportTo(EPawn->GetActorLocation() + Velocity, Rotation.Rotation()))
				Die();
			else
			{
				Velocity *= EPawn->GetFriction();
				GetWorldTimerManager().ClearTimer(UnusedHandle);
			}
		}
		else if(Velocity.Length() <= 0.01f) {
			if (bFirstInput) {
				GetWorld()->GetTimerManager().SetTimer(
					UnusedHandle, this, &AEPlayerController::Grow, EPawn->GetGrowSpeed(), true, EPawn->GetGrowSpeed());
			}
			Velocity = FVector::Zero();
		}
		if(bFirstInput && Velocity.Length() < 10.f / EPawn->GetMass())
		{
			GrowTimer++;

			if(GrowTimer > 90)
			{
				Grow();
			}
		}
		else
		{
			GrowTimer = 0;
		}
	}
}

void AEPlayerController::Die()
{
	Acceleration = FVector::Zero();
	Velocity = FVector::Zero();
	Rotation = FVector::Zero();
	bInputPressed = false; 
	bFirstInput = false;
	EPawn->OnHit(GetSpawnLocation());
	GetWorldTimerManager().ClearTimer(UnusedHandle);
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



