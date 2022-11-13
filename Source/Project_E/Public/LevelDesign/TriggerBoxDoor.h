

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TriggerBoxDoor.generated.h"


UCLASS()
class PROJECT_E_API ATriggerBoxDoor : public ATriggerBox
{
	GENERATED_BODY()

public:
	ATriggerBoxDoor();

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void CloseDoors();

	UPROPERTY(EditAnywhere)
	TArray<AActor*> LinkedDoors;

	FTimerHandle Timer;
};
