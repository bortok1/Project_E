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
	bFirstMove = false;
	ignoreFirstInputAfterReset = false;
	CurrentScale = FVector(ActorMinSize, ActorMinSize, 1);
	IsAnimating = false;
	HowManyShrink = 0;
	HowManyGrow = 0;

	SetDefaultSize();
	//Animation

	GrowCurveFloat = Owner->GrowCurveFloat;
	ShrinkCurveFloat = Owner->ShrinkCurveFloat;

	if (GrowCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("AnimationTimelineProgress"));
		GrowTimeline.AddInterpFloat(GrowCurveFloat, TimelineProgress);
		FOnTimelineEvent TimelineEndEvent;
		TimelineEndEvent.BindUFunction(this, FName("AnimationEndGrow"));
		GrowTimeline.SetTimelineFinishedFunc(TimelineEndEvent);
		GrowTimeline.SetPlayRate(2);
	}

	
	if (ShrinkCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("AnimationTimelineProgress"));
		ShrinkTimeline.AddInterpFloat(ShrinkCurveFloat, TimelineProgress);
		FOnTimelineEvent TimelineEndEvent;
		TimelineEndEvent.BindUFunction(this, FName("AnimationEndShrink"));
		ShrinkTimeline.SetTimelineFinishedFunc(TimelineEndEvent);
		ShrinkTimeline.SetPlayRate(2);
	}
}

void USizeManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Animation
	GrowTimeline.TickTimeline(DeltaTime);
	ShrinkTimeline.TickTimeline(DeltaTime);

	if(ignoreFirstInputAfterReset)
	{
		ignoreFirstInputAfterReset = false;
		return;
	}

	// Pawn is moving
	if(Movement->Velocity.Length() != 0)
	{
		if (!bFirstMove) {
			Owner->ResetTimer();
			bFirstMove = true;
		}
		GetWorld()->GetTimerManager().SetTimer(
			GrowTimeHandle, this, &USizeManagerComponent::GrowPawn, GrowSpeed, false, GrowSpeed);
	}
}

void USizeManagerComponent::GrowPawn()
{
	if (!IsAnimating)
	{
		if (HowManyGrow > 0) HowManyGrow--;
		if (CurrentScale.X < ActorMaxSize + GrowStep)
		{
			if(bFirstMove)
			{
				if (!Owner->TeleportTo(Owner->GetActorLocation() + 0.00001, Owner->GetActorRotation(), true))
				{
					Owner->Die();
				}
				GetWorld()->GetTimerManager().SetTimer(
			GrowTimeHandle, this, &USizeManagerComponent::GrowPawn, GrowSpeed, false, GrowSpeed);
			}
			
			IsAnimating = true;
			AnimationBeginScale = CurrentScale;
			AnimationEndScale = CurrentScale + FVector(GrowStep, GrowStep, 0);
			GrowTimeline.PlayFromStart();

			Mass += GrowStep / 3;
			Camera->ZoomIn();
			
			Owner->EGrowEvent();
		}
	}
	else
	{
		HowManyGrow++;
	}
}

void USizeManagerComponent::ShrinkPawn()
{
	if (!IsAnimating)
	{
		if (HowManyShrink > 0) HowManyShrink--;
		if (CurrentScale.X > ActorMinSize)
		{
			if(bFirstMove)
			{
				GetWorld()->GetTimerManager().SetTimer(
			GrowTimeHandle, this, &USizeManagerComponent::GrowPawn, GrowSpeed, false, GrowSpeed);
			}
			IsAnimating = true;
			AnimationBeginScale = CurrentScale;
			AnimationEndScale = CurrentScale - FVector(GrowStep, GrowStep, 0);
			ShrinkTimeline.PlayFromStart();

			Mass -= GrowStep / 3;
			Camera->ZoomOut();

			Owner->EShrinkEvent();
		}
	}
	else
	{
		HowManyShrink++;
	}
}

void USizeManagerComponent::AnimationTimelineProgress(float Value)
{
	FVector NewScale = FMath::Lerp(AnimationBeginScale, AnimationEndScale, Value);
	Mesh->SetRelativeScale3D(NewScale);
}

void USizeManagerComponent::AnimationEndGrow()
{
	IsAnimating = false;
	CurrentScale = AnimationEndScale;
	if (CurrentScale.X >= ActorMaxSize + GrowStep)
	{
		Owner->Die();
	}
	else if (HowManyShrink)
	{
		ShrinkPawn();
	}
	else if (HowManyGrow)
	{
		GrowPawn();
	}
}

void USizeManagerComponent::AnimationEndShrink()
{
	IsAnimating = false;
	CurrentScale = AnimationEndScale;
	if (HowManyShrink)
	{
		ShrinkPawn();
	}
	else if (HowManyGrow)
	{
		GrowPawn();
	}
}

void USizeManagerComponent::SetDefaultSize()
{
	Mesh->SetRelativeScale3D(FVector(ActorMinSize, ActorMinSize, 1));
	CurrentScale = FVector(ActorMinSize, ActorMinSize, 1);
	AnimationBeginScale = FVector(ActorMinSize, ActorMinSize, 1);
	HowManyShrink = 0;
	HowManyGrow = 0;
	IsAnimating = 0;
	GrowTimeline.Stop();
	ShrinkTimeline.Stop();

	Mass = DefaultMass;
	
	// Timer exists, but is not ticking
	GetWorld()->GetTimerManager().ClearTimer(GrowTimeHandle);
	
	Owner->StopTimer();
	bFirstMove = false;
	ignoreFirstInputAfterReset = true;
}

