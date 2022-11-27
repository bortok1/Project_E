// Copyright Bean Bin, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EHealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_E_API UEHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/** Brodcasting delegate on death */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath OnDeath;

	// Sets default values for this component's properties
	UEHealthComponent();

	UFUNCTION()
	void BrodcastDeath();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
