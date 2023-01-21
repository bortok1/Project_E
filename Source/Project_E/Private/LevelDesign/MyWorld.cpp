// Copyright Bean Bin, All Rights Reserved.

#include "LevelDesign/MyWorld.h"
#include "EngineUtils.h"
#include "Character/EPawn.h"
#include "LevelDesign/PickUp.h"
#include "Ice.h"

// Sets default values
AMyWorld::AMyWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AMyWorld::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (nullptr != Cast<AEPawn>(*ActorItr))
			player = Cast<AEPawn>(*ActorItr);
		else if (nullptr != Cast<APickUpSpawner>(*ActorItr))
			pickUpSpawners.push_back(Cast<APickUpSpawner>(*ActorItr));
		else if (nullptr != Cast<AIce>(*ActorItr))
			ice.push_back(Cast<AIce>(*ActorItr));
	}
    if(player != nullptr)
    	player->OnDeath.AddUniqueDynamic(this, &AMyWorld::Clear);
}

// Called every frame
void AMyWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMyWorld::Clear()
{
	APickUp* xyz;
	int nr = pickUps.size();
	for(int i = 0; i < nr; i++)
	{
		pickUps[i]->Destroy();
		//TODO B³¹d tu wyskakuje jak uroœnie na lodzie
	}
	
	for(int i = 0; i < pickUpSpawners.size(); i++)
	{
		xyz = pickUpSpawners[i]->SpawnPickUp();
		if(xyz != nullptr)
			pickUps.push_back(xyz);
	}

	for (int i = 0; i < ice.size(); i++) {
		ice[i]->Reset();
	}
	
}

void AMyWorld::ListPickUps()
{
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (nullptr != Cast<APickUp>(*ActorItr))
			pickUps.push_back(Cast<APickUp>(*ActorItr));
	}
}
