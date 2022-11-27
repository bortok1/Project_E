// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "EPawn.generated.h"


class AEPlayerController;
class UECameraComponent;

UCLASS()
class PROJECT_E_API AEPawn : public APawn
{
	GENERATED_BODY()
public:

	UPROPERTY()
	class UEHealthComponent* HealthComponent;

	AEPawn();

	// Components
	UPROPERTY(EditAnywhere)
	class UEFloatingPawnMovement* MovementComponent;

	UPROPERTY(EditAnywhere)
	class USizeManagerComponent* SizeComponent;

	// Mesh == RootComponent
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* CharacterMesh;

	// Top down camera
	UPROPERTY(EditAnywhere)
	class UECameraComponent* Camera;

	// Camera boom positioning the camera above the character
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* CameraBoom;

	FVector2D GetMousePosition() const;

	void Die();
	
private:
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	void Win();

	void Move(const FInputActionValue& ActionValue);
	
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
		UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult);

	UFUNCTION()
	void BeginEarthOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
		UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);

	UFUNCTION()
	void EndEarthOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
		UPrimitiveComponent* PrimitiveComponent1, int I);

	bool bStopMeNow;
	
	UPROPERTY()
	AEPlayerController* EPlayerController;

private:
	UFUNCTION(BlueprintCallable)
	bool ResetTimer();

	bool StopTimer();

	bool WriteScoreTimer();

public:
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* TimerWidgetRef;

public:
	void SetStopMeNow(bool const bNewStopMeNow) { bStopMeNow = bNewStopMeNow; }
	
	UECameraComponent* GetTopDownCameraComponent() const { return Camera; }
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	[[nodiscard]] bool GetStopMeNow() const { return bStopMeNow; }
};



