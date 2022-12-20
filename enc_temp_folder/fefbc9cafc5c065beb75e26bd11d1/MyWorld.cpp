// Copyright Bean Bin, All Rights Reserved.

#include "LevelDesign/MyWorld.h"
#include "EngineUtils.h"
#include "Character/EPawn.h"
#include "LevelDesign/PickUp.h"

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
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr )
    	{
    		if(nullptr != Cast<AEPawn>(*ActorItr))
    			player = Cast<AEPawn>(*ActorItr);
    		else if(nullptr != Cast<APickUp>(*ActorItr))
    			pickUps.push_back(Cast<APickUp>(*ActorItr));
    		else if(nullptr != Cast<APickUpSpawner>(*ActorItr))
    			pickUpSpawners.push_back(Cast<APickUpSpawner>(*ActorItr));
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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("CLEEAR")));
	APickUp* xyz;
	int nr = pickUps.size();
	for(int i = 0; i < nr; i++)
	{
		xyz = pickUps.back();
		xyz->Destroy();
	}
	
	for(int i = 0; i < pickUpSpawners.size(); i++)
	{
		xyz = pickUpSpawners[i]->SpawnPickUp();
		if(xyz != nullptr)
			pickUps.push_back(xyz);
	}
	
}