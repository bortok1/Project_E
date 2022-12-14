// Copyright Bean Bin, All Rights Reserved.


#include "LevelDesign/DeathTrigger.h"
#include "Character/EPawn.h"
#include "Components/ShapeComponent.h"

ADeathTrigger::ADeathTrigger()
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADeathTrigger::OnComponentBeginOverlap);
}

void ADeathTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEPawn* Pawn = Cast<AEPawn>(OtherActor))
	{
		Pawn->Die();
	}
}