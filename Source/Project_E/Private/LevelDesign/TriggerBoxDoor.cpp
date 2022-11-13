// Copyright Bean Bin, All Rights Reserved.


#include "LevelDesign/TriggerBoxDoor.h"
#include "Components/ShapeComponent.h"
#include "Character/EPawn.h"

ATriggerBoxDoor::ATriggerBoxDoor()
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATriggerBoxDoor::OnComponentBeginOverlap);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &ATriggerBoxDoor::OnComponentEndOverlap);
}

void ATriggerBoxDoor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	int i = 0;
	if (AEPawn* Pawn = Cast<AEPawn>(OtherActor)) {
		for (AActor* Door : LinkedDoors) {
			if (Door) {
				// Hides visible components
				Door->SetActorHiddenInGame(true);

				// Disables collision components
				Door->SetActorEnableCollision(false);
			}
			i++;
		}
	}
}

void ATriggerBoxDoor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AEPawn* Pawn = Cast<AEPawn>(OtherActor)) {
		FVector Scale = Pawn->GetActorRelativeScale3D();
		float ScaleX = Scale[0];
		float duration;
		if (ScaleX == 1) { duration = .5f; }
		else if (ScaleX == 2) { duration = 2.0f; }
		else if (ScaleX == 3) { duration = 4.0f; }
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

