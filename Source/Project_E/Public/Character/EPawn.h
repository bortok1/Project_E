// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/EMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "EPawn.generated.h"

UCLASS()
class PROJECT_E_API AEPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* CharacterMesh;

	UPROPERTY()
	class UBoxComponent* CollisionBox;

	UPROPERTY()
	UEMovementComponent* EMovementComponent;

	UPROPERTY(Category="Movement", EditAnywhere)
	float Mass = 1.f;
	
	UPROPERTY(Category="Movement", EditAnywhere)
	float AngularDumping = 20.f;
	
	UPROPERTY(Category="Movement", EditAnywhere)
	float Speed = .01f;
	
	UPROPERTY(Category="Movement", EditAnywhere)
	float Friction = 0.95f;
	
public:
	[[nodiscard]] float GetMass() const {return Mass;}

	[[nodiscard]] float GetAngularDumping() const {return AngularDumping;}

	[[nodiscard]] float GetSpeed() const {return Speed;}

	[[nodiscard]] float GetFriction() const {return Friction;}

};
