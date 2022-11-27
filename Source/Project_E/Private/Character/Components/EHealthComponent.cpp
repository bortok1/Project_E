// Copyright Bean Bin, All Rights Reserved.
#include "Character/Components/EHealthComponent.h"

// Sets default values for this component's properties
UEHealthComponent::UEHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UEHealthComponent::BrodcastDeath()
{
	OnDeath.Broadcast();
}


// Called when the game starts
void UEHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UEHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

