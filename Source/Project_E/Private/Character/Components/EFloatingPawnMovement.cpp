// Copyright Bean Bin, All Rights Reserved.


#include "Character/Components/EFloatingPawnMovement.h"
#include "Character/EPawn.h"

UEFloatingPawnMovement::UEFloatingPawnMovement()
{
	GravityForce = 478.f;
	MoveScale = 1.f;
	AngularDumping = 20.f;
}

void UEFloatingPawnMovement::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AEPawn>(GetOwner());
	Owner->OnDeath.AddUniqueDynamic(this, &UEFloatingPawnMovement::StopImmediately);
}

void UEFloatingPawnMovement::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Gravity(DeltaTime);
}

void UEFloatingPawnMovement::StopImmediately()
{
	this->StopMovementImmediately();
}

void UEFloatingPawnMovement::Gravity(float DeltaTime)
{
	JumpVelocity += GravityForce * DeltaTime;

	FVector TeleportLocation = Owner->GetActorLocation();
	TeleportLocation.Z -= JumpVelocity * DeltaTime + 0.5 * GravityForce * DeltaTime * DeltaTime;
	
	if (Owner->TeleportTo(TeleportLocation, Owner->GetActorRotation(), true)) {
		Owner->TeleportTo(TeleportLocation, Owner->GetActorRotation(), false, true);
	}
	else {
		JumpVelocity = GravityForce * DeltaTime;
	}
}

FVector UEFloatingPawnMovement::GetVectorTowardsCursor(FVector2D CursorLocation)
{
	// Take half of screen resolution
	FVector2D HalfResolution = FVector2D::Zero();
	if ( GEngine && GEngine->GameViewport )
	{
		GEngine->GameViewport->GetViewportSize(HalfResolution);
		HalfResolution *= 0.5f;
	}

	// Direction from screen center (player) to mouse cursor
	return FVector((CursorLocation - HalfResolution).GetSafeNormal(),0);
}

