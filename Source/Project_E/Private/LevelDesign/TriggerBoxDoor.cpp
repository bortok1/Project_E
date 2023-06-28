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
		const float ScaleX = Pawn->GetActorRelativeScale3D().X;
		float duration;

		if (ScaleX < 1.5f) { duration = .5f; }
		else if (ScaleX < 2.5f) { duration = 2.0f; }
		else if (ScaleX < 3.5f) { duration = 4.0f; }
		else { duration = 8.0f; }
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

