// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "ECameraComponent.generated.h"

class AEPawn;
UCLASS()
class PROJECT_E_API UECameraComponent : public UCameraComponent
{
	GENERATED_BODY()

	UECameraComponent();
	
public:
	void SetDefaultFieldOfView();
	void ZoomIn();
	void ZoomOut();
	void MoveCamera(FVector2d CursorLocation);

private:
	UPROPERTY()
	AEPawn* Owner;

public:
	
	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
	float FOVStep = 10.f;

	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "180.0", UIMin = "0.0", UIMax = "180.0"))
	float DefaultFOV = 90.f;

	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "300.0", UIMin = "000.0", UIMax = "300.0"))
	float CameraMovementDivider = 150.f;

	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "300.0", UIMin = "0.0", UIMax = "300.0"))
	float CameraMovementMultiplier = 10.f;
	
};
