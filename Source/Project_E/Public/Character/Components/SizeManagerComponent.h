// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "Components/TimelineComponent.h"
#include "SizeManagerComponent.generated.h"

class uCurveFloat;
class AEPawn;
class UECameraComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_E_API USizeManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	
	// Sets default values for this component's properties
	USizeManagerComponent();
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:

	UFUNCTION()
	void AnimationTimelineProgress(float Value);

	UFUNCTION()
	void AnimationEndGrow();

	UFUNCTION()
	void AnimationEndShrink();

	void SetDefaultSize();

	UFUNCTION(BlueprintCallable)
	void GrowPawn();
	
	UFUNCTION(BlueprintCallable)
	void ShrinkPawn();


	UPROPERTY()
	UCurveFloat* GrowCurveFloat;

	UPROPERTY()
	UCurveFloat* ShrinkCurveFloat;
	
	UPROPERTY(BlueprintReadOnly)
	FVector AnimationEndScale;

private:

	FTimeline GrowTimeline, ShrinkTimeline;

	FVector AnimationBeginScale;

	FVector CurrentScale;

	bool IsAnimating;

	int HowManyShrink, HowManyGrow;

	bool bFirstMove;
	bool ignoreFirstInputAfterReset;

	UPROPERTY()
	AEPawn* Owner;

	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	class UEFloatingPawnMovement* Movement;
	
	UPROPERTY()
	UECameraComponent* Camera;
	
	/** If equal or lower cube can't get smaller*/
	UPROPERTY(Category = "Mesh", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
	float ActorMinSize = 1.f;

	/** If equal or higer cube can't grow*/
	UPROPERTY(Category = "Mesh", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
	float ActorMaxSize = 4.f;

	/** How much the cube grow every instance (added value) */
	UPROPERTY(Category = "Mesh", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "10.0", UIMin = "0.0", UIMax = "10.0"))
	float GrowStep = 1.f;

	UPROPERTY(Category = "Mesh", EditAnywhere, meta = (ClampMin = 0.0001, UIMin = 0.0001))
	float DefaultMass = 1.f;

	UPROPERTY(Category = "Movement", EditAnywhere, meta = (ClampMin = 0.0001, UIMin = 0.0001))
	float Mass = DefaultMass;
	
	UPROPERTY(Category = "Time", EditAnywhere, meta = (ClampMin = 0.0001, ClampMax = 1, UIMin = 0.0001, UIMax = 1))
	float GrowSpeed = 1.f;

public:
	[[nodiscard]]float GetMass() const { return Mass; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FTimerHandle GrowTimeHandle;
};
