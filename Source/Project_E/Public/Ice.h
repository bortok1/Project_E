// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ice.generated.h"

UCLASS()
class PROJECT_E_API AIce : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIce();

	UFUNCTION(BlueprintImplementableEvent)
		void EIceResetEvent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Reset();

};
