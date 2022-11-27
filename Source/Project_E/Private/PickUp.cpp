// Copyright Bean Bin, All Rights Reserved.


#include "PickUp.h"
#include "Character/Components/EHealthComponent.h"

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickUp::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	HealthComponent = this->FindComponentByClass<UEHealthComponent>();
	if (HealthComponent != nullptr) {
		HealthComponent->OnDeath.AddUniqueDynamic(this, &APickUp::DestroyPU);
	}
}

void APickUp::DestroyPU()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Destroy");
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

