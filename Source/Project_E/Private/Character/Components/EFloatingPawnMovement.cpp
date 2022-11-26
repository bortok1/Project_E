// Copyright Bean Bin, All Rights Reserved.


#include "Character/Components/EFloatingPawnMovement.h"

#include "Character/EPawn.h"
#include "Kismet/GameplayStatics.h"

void UEFloatingPawnMovement::BeginPlay()
{
	Super::BeginPlay();
	
	MoveScale = 1.f;
	AngularDumping = 20.f;

	Owner = Cast<AEPawn>(GetOwner());
}

void UEFloatingPawnMovement::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Owner->GetStopMeNow())
	{
		Owner->SetStopMeNow(false);
		this->StopMovementImmediately();
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

