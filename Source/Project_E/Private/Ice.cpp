// Copyright Bean Bin, All Rights Reserved.


#include "Ice.h"

// Sets default values
AIce::AIce()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AIce::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AIce::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIce::Reset()
{
	EIceResetEvent();
}

