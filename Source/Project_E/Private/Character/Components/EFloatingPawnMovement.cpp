// Copyright Bean Bin, All Rights Reserved.


#include "Character/Components/EFloatingPawnMovement.h"
#include "Character/EPawn.h"

UEFloatingPawnMovement::UEFloatingPawnMovement()
{
	GravityForce = 100.f;
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

void UEFloatingPawnMovement::Gravity(float DeltaTime) const
{
	FVector TeleportLocation = Owner->GetActorLocation();
	TeleportLocation.Z -= GravityForce * DeltaTime;
	Owner->TeleportTo(TeleportLocation, Owner->GetActorRotation());
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

