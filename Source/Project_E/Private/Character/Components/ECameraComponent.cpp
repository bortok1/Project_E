// Copyright Bean Bin, All Rights Reserved.


#include "Character/Components/ECameraComponent.h"
#include "Character/EPawn.h"


UECameraComponent::UECameraComponent()
{
	Owner = Cast<AEPawn>(GetOwner());
	PrimaryComponentTick.bCanEverTick = true;
	EPlayerController = Cast<AEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CameraShakeClass = UECameraShake::StaticClass();
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
	
	TargetPosition = FVector(BoomZ, NewCameraLocation.X, -NewCameraLocation.Y);
}

void UECameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector2D Mouse = FVector2d::Zero();
	EPlayerController->GetMousePosition(Mouse.X, Mouse.Y);
	MoveCamera(Mouse);

	FVector location = GetRelativeLocation();
	FVector vector = TargetPosition - location;
	FVector2D vector2D;
	vector2D.X = vector.Y;
	vector2D.Y = -vector.Z;
	float length = vector2D.Length();
	length = FMath::Min(length, CameraMaxSpeedDistance);
	length /= CameraMaxSpeedDistance;
	float speed = CameraMaxSpeed * length;
	if (vector2D.Length() > speed * DeltaTime)
	{
		vector2D.Normalize();
		vector2D *= speed;
		vector2D *= DeltaTime;
		location.Y += vector2D.X;
		location.Z -= vector2D.Y;
		this->SetRelativeLocation(location);
	}
}

void UECameraComponent::Shake()
{
	EPlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass, 1.0f);
}
