// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "EPawn.generated.h"


struct FInputActionValue;
class AEPlayerController;
class UECameraComponent;
class UNiagaraSystem;

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
	void ResetLevel();

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> ADeathMark;

	// BlueprintEvents
	UFUNCTION(BlueprintImplementableEvent)
	void EWinEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void EAnimationsDoneEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void EDiedEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void EGrowEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void EShrinkEvent();
	
private:
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Win();

	void Move(const FInputActionValue& ActionValue);
	
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
		UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult);

	// Where player Begins play
	FVector StartPosition;

	UPROPERTY()
	AEPlayerController* EPlayerController;

	UFUNCTION()
	void SpawnObject(FVector loc, FRotator rot) const;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* NS_Particles;

	bool isAnimationPlaying;
private:
	UFUNCTION(BlueprintCallable)
	bool ResetTimer();

	bool StopTimer();

	bool WriteScoreTimer();

public:
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* TimerWidgetRef;

public:
	UECameraComponent* GetTopDownCameraComponent() const { return Camera; }
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};



