// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "Camera/CameraComponent.h"
#include "ECameraComponent.generated.h"

class UMatineeCameraShake;
class AEPawn;
UCLASS()
class PROJECT_E_API UECameraComponent : public UCameraComponent
{
	GENERATED_BODY()

	UECameraComponent();
	
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetDefaultFieldOfView();
	void ZoomIn();
	void ZoomOut();
	void Shake() const;

private:
	void MoveCamera();

	UPROPERTY()
	AEPawn* Owner;

	UPROPERTY()
	APlayerCameraManager* CameraManager;
	
	FVector TargetPosition;
	
	TSubclassOf<UMatineeCameraShake> CameraShakeClass;

public:
	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
	float FOVStep = 10.f;

	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "180.0", UIMin = "0.0", UIMax = "180.0"))
	float DefaultFOV = 90.f;

	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "300.0", UIMin = "000.0", UIMax = "300.0"))
	float CameraMovementDivider = 150.f;

	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "300.0", UIMin = "0.0", UIMax = "300.0"))
	float CameraMovementMultiplier = 10.f;

	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "10.0", ClampMax = "300.0", UIMin = "10.0", UIMax = "300.0"))
	float CameraMaxSpeed = 200.f;

	//when distance between current camera location and target camera location is equal/greater than CameraMaxSpeedDistance, camera has max speed
	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "5.0", ClampMax = "50.0", UIMin = "5.0", UIMax = "50.0"))
	float CameraMaxSpeedDistance = 10.f;
};
