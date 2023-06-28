// Copyright Bean Bin, All Rights Reserved.


#include "Character/Components/EFloatingPawnMovement.h"
#include "Character/EPawn.h"
#include "Character/Components/SizeManagerComponent.h"

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
	LastJoystickVector = FVector(ScreenResolution * 0.5f, 0);
}

void UEFloatingPawnMovement::Move(float DeltaTime, bool bMoveInputPressed = false)
{
	Gravity(DeltaTime);

	if(bMoveInputPressed && !isAirTime)
	{
		LastJoystickVector = FVector(GetThumbstickPosition().GetSafeNormal(), 0);
		Owner->AddMovementInput(LastJoystickVector, MoveSpeed/Owner->SizeComponent->GetMass());
		Owner->CharacterMesh->SetRelativeRotation(LastJoystickVector.Rotation());
		bStopAfterReset = false;
	}
	else if(isAirTime && !bStopAfterReset){
		Owner->AddMovementInput(LastJoystickVector, MoveSpeed/Owner->SizeComponent->GetMass());
		Owner->CharacterMesh->SetRelativeRotation(FVector(GetThumbstickPosition(), 0).Rotation());
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
		LastJoystickVector -= LastJoystickVector * SpeedDumpWhileAirTime * DeltaTime;
	}
	else {
		JumpVelocity = GravityForce * DeltaTime;
		isAirTime = false;
	}
}

//For mobile settings
FVector2D UEFloatingPawnMovement::GetThumbstickPosition() const
{
	return Owner->GetJoystickAxis();
}