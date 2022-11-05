// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EPawn.generated.h"

UCLASS()
class PROJECT_E_API AEPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEPawn();

	void GrowOnStop();

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* CharacterMesh;

public:
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** If equal or lower cube can't get smaller*/
	UPROPERTY(Category="Growth", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
	float ActorMinSize = 2.f;

	/** If equal or higer cube can't grow*/
	UPROPERTY(Category="Growth", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
	float ActorMaxSize = 8.f;

	/** How much the cube grow every instance (added value) */
	UPROPERTY(Category="Growth", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "10.0", UIMin = "0.0", UIMax = "10.0"))
	float GrowStep = 1.f;

	UPROPERTY(Category="Movement", EditAnywhere, meta = (ClampMin = 0.0001, UIMin = 0.0001))
	float Mass = 1.f;
	
	UPROPERTY(Category="Movement", EditAnywhere, meta = (ClampMin = 0.0001, UIMin = 0.0001))
	float AngularDumping = 20.f;
	
	UPROPERTY(Category="Movement", EditAnywhere, meta = (ClampMin = 0.0001, UIMin = 0.0001))
	float Speed = .01f;
	
	UPROPERTY(Category="Movement", EditAnywhere, meta = (ClampMin = 0.0001, ClampMax = 1, UIMin = 0.0001, UIMax = 1))
	float Friction = 0.95f;
	
public:
	[[nodiscard]] float GetMass() const {return Mass;}
	[[nodiscard]] float GetAngularDumping() const {return AngularDumping;}
	[[nodiscard]] float GetSpeed() const {return Speed;}
	[[nodiscard]] float GetFriction() const {return Friction;}
	[[nodiscard]] float GetGrowStep() const {return GrowStep;}
};
