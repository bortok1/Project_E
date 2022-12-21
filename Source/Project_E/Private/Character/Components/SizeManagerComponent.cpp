// Copyright Bean Bin, All Rights Reserved.


#include "Character/Components/SizeManagerComponent.h"
#include "Character/EPawn.h"
#include "Character/Components/ECameraComponent.h"
#include "Character/Components/EFloatingPawnMovement.h"

USizeManagerComponent::USizeManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USizeManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AEPawn>(GetOwner());
	Mesh = Owner->CharacterMesh;
	Movement = Owner->MovementComponent;
	Camera = Owner->Camera;

	// Timer exists, but is not ticking
	GetWorld()->GetTimerManager().SetTimer(
		GrowTimeHandle, this, &USizeManagerComponent::GrowPawn, GrowSpeed, false, GrowSpeed);
	GetWorld()->GetTimerManager().PauseTimer(GrowTimeHandle);
}

void USizeManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Pawn is moving
	if(Movement->Velocity.Length() != 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(GrowTimeHandle);
	}
	// Pawn is not moving && timer exists
	else if(Movement->Velocity.Length() == 0 && !GetWorld()->GetTimerManager().TimerExists(GrowTimeHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
		GrowTimeHandle, this, &USizeManagerComponent::GrowPawn, GrowSpeed, false, GrowSpeed);
	}
}

void USizeManagerComponent::GrowPawn()
{
	if (Mesh->GetRelativeScale3D().X < ActorMaxSize)
	{
		Mesh->SetRelativeScale3D(Mesh->GetRelativeScale3D() + FVector(GrowStep, GrowStep, 0));

		if(!Owner->TeleportTo(Owner->GetActorLocation() * 1.00001, Owner->GetActorRotation(), true))
		{
			Owner->Die();
		}
		
		Mass += GrowStep / 3;
		Camera->ZoomIn();
		
		GetWorld()->GetTimerManager().ClearTimer(GrowTimeHandle);

		Owner->EGrowEvent();
	}
	else
	{
		Owner->Die();
	}
}

void USizeManagerComponent::ShrinkPawn()
{
	if (Mesh->GetRelativeScale3D().X > ActorMinSize)
	{
		Mesh->SetRelativeScale3D(Mesh->GetRelativeScale3D() - FVector(GrowStep, GrowStep, 0));
		Mass -= GrowStep / 3;
		Camera->ZoomOut();

		GetWorld()->GetTimerManager().ClearTimer(GrowTimeHandle);

		Owner->EShrinkEvent();
	}
}

void USizeManagerComponent::SetDefaultSize()
{
	Mesh->SetRelativeScale3D(FVector(ActorMinSize, ActorMinSize, 1));
	Mass = DefaultMass;
	
	// Timer exists, but is not ticking
	GetWorld()->GetTimerManager().SetTimer(
		GrowTimeHandle, this, &USizeManagerComponent::GrowPawn, GrowSpeed, false, GrowSpeed);
	GetWorld()->GetTimerManager().PauseTimer(GrowTimeHandle);
}

