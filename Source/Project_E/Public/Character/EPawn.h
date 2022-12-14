// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "EPawn.generated.h"


struct FInputActionValue;
class AEPlayerController;
class UECameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS()
class PROJECT_E_API AEPawn : public APawn
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FOnDeath OnDeath;

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

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> ActorToSpawn;

	// BlueprintEvents
	UFUNCTION(BlueprintImplementableEvent)
	void EDiedEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void EGrowEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void EShrinkEvent();
	
private:
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Win();

	void Move(const FInputActionValue& ActionValue);
	
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
		UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult);

	// Where player Begins play
	FVector StartPosition;
	
	bool bStopMeNow;
	
	UPROPERTY()
	AEPlayerController* EPlayerController;

	UFUNCTION()
	void SpawnObject(FVector loc, FRotator rot);

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



