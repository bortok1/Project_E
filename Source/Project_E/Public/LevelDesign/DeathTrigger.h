// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "Engine/TriggerBox.h"
#include "DeathTrigger.generated.h"

UCLASS()
class PROJECT_E_API ADeathTrigger : public ATriggerBox
{
	GENERATED_BODY()
public:
	ADeathTrigger();
	
private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	FTimerHandle Timer;
};
