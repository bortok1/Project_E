// Copyright Bean Bin, All Rights Reserved.


#include "Character/Components/ECameraComponent.h"
#include "Character/EPawn.h"
#include "Character/Components/ECameraShake.h"
#include "Kismet/GameplayStatics.h"


UECameraComponent::UECameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CameraShakeClass = UECameraShake::StaticClass();
}

void UECameraComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AEPawn>(GetOwner());
	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
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

void UECameraComponent::MoveCamera()
{
	FVector2d JoystickVector = Owner->GetJoystickAxis();
	const float VectorLength = JoystickVector.Length();
	JoystickVector.Normalize();
	
	const FVector2D NewCameraLocation = JoystickVector *
		FMath::Log2(VectorLength/CameraMovementDivider + 1) *
			CameraMovementMultiplier;

	const float BoomZ = Owner->CameraBoom->GetComponentToWorld().GetLocation().Z;
	
	TargetPosition = FVector(BoomZ, NewCameraLocation.X, -NewCameraLocation.Y);
}

void UECameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveCamera();

	FVector location = GetRelativeLocation();
	const FVector vector = TargetPosition - location;
	
	FVector2D vector2D;
	vector2D.X = vector.Y;
	vector2D.Y = -vector.Z;
	
	float length = vector2D.Length();
	length = FMath::Min(length, CameraMaxSpeedDistance);
	length /= CameraMaxSpeedDistance;

	const float speed = CameraMaxSpeed * length * DeltaTime;
	if (length > speed)
	{
		vector2D.Normalize();
		vector2D *= speed;
		vector2D *= DeltaTime;
		location.Y += vector2D.X;
		location.Z -= vector2D.Y;
		this->SetRelativeLocation(location);
	}
}

void UECameraComponent::Shake() const
{
	CameraManager->StartCameraShake(CameraShakeClass, 1.0f);
}
