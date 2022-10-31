// Copyright Bean Bin, All Rights Reserved.

#include "LevelDesign/SimpleTeleporter.h"
#include "GameFramework/Character.h"

// Sets default values
ASimpleTeleporter::ASimpleTeleporter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(CollisionMesh);
	
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ASimpleTeleporter::OnBoxBeginOverlap);
}

void ASimpleTeleporter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASimpleTeleporter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<ACharacter>(OtherActor))
	{
		OtherActor->SetActorLocation(NewLocation);
	}
}

