// Copyright Bean Bin, All Rights Reserved.

#include "LevelDesign/SimpleTeleporter.h"
#include "Character/EPawn.h"
#include "Components/BoxComponent.h"

// Sets default values
ASimpleTeleporter::ASimpleTeleporter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(CollisionMesh);
	
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ASimpleTeleporter::OnBoxBeginOverlap);
}

void ASimpleTeleporter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<AEPawn>(OtherActor))
	{
		OtherActor->TeleportTo(NewLocation, OtherActor->GetActorRotation());
	}
}

