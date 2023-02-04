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

	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void Tick(float DeltaTime) override;

public:
	AEPawn();
	
	// Delegate
	UPROPERTY()
	FOnDeath OnDeath;

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
	
	// Components
	UPROPERTY(BlueprintReadOnly)
	class USizeManagerComponent* SizeComponent;

	UPROPERTY(BlueprintReadWrite)
	class UEFloatingPawnMovement* MovementComponent;
	
	// Mesh == RootComponent
	UPROPERTY()
	class UStaticMeshComponent* CharacterMesh;

	// Top down camera
	UPROPERTY()
	class UECameraComponent* Camera;

	// Camera boom positioning the camera above the character
	UPROPERTY()
	class USpringArmComponent* CameraBoom;

	UPROPERTY()
	AEPlayerController* EPlayerController;

	void Die();
	
private:
	// Actor spawned at place of death
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> ADeathMark;

	UFUNCTION()
	void SpawnDeathMark(FVector loc, FRotator rot) const;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* NS_Particles;

	bool isAnimationPlaying;
	
	// Where player Begins play
	FVector StartPosition;
	
	void ResetLevel();

	UFUNCTION(BlueprintCallable)
	void Win();

	void MoveEvent(const struct FInputActionValue& ActionValue);
	bool bMoveInputPressed;
	
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
		UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult);
	


public:	
	UFUNCTION(BlueprintCallable)
	bool ResetTimer();

	bool StopTimer();

	bool WriteScoreTimer();

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* TimerWidgetRef;
	
	[[nodiscard]] UECameraComponent* GetTopDownCameraComponent() const { return Camera; }
	[[nodiscard]] USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
