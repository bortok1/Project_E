// Copyright Bean Bin, All Rights Reserved.


#include "Character/Components/ECameraComponent.h"

#include "Character/EPawn.h"


UECameraComponent::UECameraComponent()
{
	Owner = Cast<AEPawn>(GetOwner());
}

void UECameraComponent::SetDefaultFieldOfView()
{
	SetFieldOfView(DefaultFOV);
}

void UECameraComponent::ZoomIn()
{
	SetFieldOfView(FieldOfView + FOVStep);
}

void UECameraComponent::ZoomOut()
{
	SetFieldOfView(FieldOfView - FOVStep);
}

void UECameraComponent::MoveCamera(FVector2d CursorLocation)
{
	// Take half of screen resolution
	FVector2D HalfResolution = FVector2D::Zero();
	if ( GEngine && GEngine->GameViewport )
	{
		GEngine->GameViewport->GetViewportSize(HalfResolution);
		HalfResolution *= 0.5f;
	}

	FVector2D Vector = CursorLocation - HalfResolution;
	const float VectorLength = Vector.Length();
	Vector.Normalize();
	const FVector2D NewCameraLocation = Vector * FMath::Log2(VectorLength/CameraMovementDivider + 1) * CameraMovementMultiplier;

	const float BoomZ = Owner->CameraBoom->GetComponentToWorld().GetLocation().Z;
	
	this->SetRelativeLocation(FVector(BoomZ, NewCameraLocation.X, -NewCameraLocation.Y));
}
