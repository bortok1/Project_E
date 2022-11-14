// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "EPawn.generated.h"

UCLASS()
class PROJECT_E_API AEPawn : public APawn
{
	GENERATED_BODY()
public:
	AEPawn();

	UFUNCTION()
		void OnHit(FVector StartLocation);


	UFUNCTION()
		void OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION()
		bool GrowBox();

	UFUNCTION(BlueprintCallable)
		bool ShrinkBox();

	UFUNCTION(BlueprintCallable)
		bool ResetTimer();

	bool StopTimer();

	bool WriteScoreTimer();

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* CharacterMesh;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TimerWidget;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* TimerWidgetRef;

private:
	/** If equal or lower cube can't get smaller*/
	UPROPERTY(Category = "Growth", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
		float ActorMinSize = 1.f;

	/** If equal or higer cube can't grow*/
	UPROPERTY(Category = "Growth", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
		float ActorMaxSize = 4.f;

	/** How much the cube grow every instance (added value) */
	UPROPERTY(Category = "Growth", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "10.0", UIMin = "0.0", UIMax = "10.0"))
		float GrowStep = 1.f;

	UPROPERTY(Category = "Growth", EditAnywhere, meta = (ClampMin = 0.0001, ClampMax = 1, UIMin = 0.0001, UIMax = 1))
		float GrowSpeed = 0.05f;

	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "180.0", UIMin = "0.0", UIMax = "180.0"))
		float DefaultFOV = 90.f;

	/** How much Field Of View grow every instance) */
	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
		float FOVStep = 10.f;

	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "300.0", UIMin = "000.0", UIMax = "300.0"))
		float CameraMovementDivider = 150.f;

	UPROPERTY(Category = "Camera", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "300.0", UIMin = "0.0", UIMax = "300.0"))
		float CameraMovementMultiplier = 50.f;

	UPROPERTY(Category = "Movement", EditAnywhere, meta = (ClampMin = 0.0001, UIMin = 0.0001))
		float DefaultMass = 1.f;

	UPROPERTY(Category = "Movement", EditAnywhere, meta = (ClampMin = 0.0001, UIMin = 0.0001))
		float Mass = DefaultMass;

	UPROPERTY(Category = "Movement", EditAnywhere, meta = (ClampMin = 0.0001, UIMin = 0.0001))
		float AngularDumping = 20.f;

	UPROPERTY(Category = "Movement", EditAnywhere, meta = (ClampMin = 0.0001, UIMin = 0.0001))
		float Speed = .01f;

	UPROPERTY(Category = "Movement", EditAnywhere, meta = (ClampMin = 0.0001, ClampMax = 1, UIMin = 0.0001, UIMax = 1))
		float Friction = 0.95f;

	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UBoxComponent* BoxCollider = nullptr;

public:
	[[nodiscard]] float GetMass() const { return Mass; }
	[[nodiscard]] float GetAngularDumping() const { return AngularDumping; }
	[[nodiscard]] float GetSpeed() const { return Speed; }
	[[nodiscard]] float GetFriction() const { return Friction; }
	[[nodiscard]] float GetGrowStep() const { return GrowStep; }
	[[nodiscard]] float GetGrowSpeed() const { return GrowSpeed; }
	[[nodiscard]] float GetCameraMovementDivider() const { return CameraMovementDivider; }
	[[nodiscard]] float GetCameraMovementMultiplier() const { return CameraMovementMultiplier; }
};
