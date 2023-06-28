// Copyright Bean Bin, All Rights Reserved.

#include "LevelDesign/TriggerBoxDoor.h"
#include "Character/EPawn.h"
#include "Components/ShapeComponent.h"

ATriggerBoxDoor::ATriggerBoxDoor()
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATriggerBoxDoor::OnComponentBeginOverlap);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &ATriggerBoxDoor::OnComponentEndOverlap);
}

void ATriggerBoxDoor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<AEPawn>(OtherActor))
		return;
	
	for (AActor* Door : LinkedDoors) {
		if (Door) {
			// Hides visible components
			Door->SetActorHiddenInGame(true);

			// Disables collision components
			Door->SetActorEnableCollision(false);
		}
	}
}

void ATriggerBoxDoor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (const AEPawn* Pawn = Cast<AEPawn>(OtherActor)) {
		FVector Scale = Pawn->GetActorRelativeScale3D();
		float duration;

		switch(static_cast<int>(Scale[0]))
		{
		case 1: duration = .5f; break;
		case 2: duration = 2.f; break;
		case 3: duration = 4.f; break;
		default: duration = 8.f;
		}
		GetWorldTimerManager().SetTimer(Timer, this, &ATriggerBoxDoor::CloseDoors, duration, false);
	}
}

void ATriggerBoxDoor::CloseDoors()
{
	for (AActor* Door : LinkedDoors) {
		if (Door) {
			// Hides visible components
			Door->SetActorHiddenInGame(false);

			// Disables collision components
			Door->SetActorEnableCollision(true);
		}
	}
}

