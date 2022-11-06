// Copyright Bean Bin, All Rights Reserved.


#include "Character/EPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AEPawn::AEPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StartLocation = FVector(1200.f, 1500.f, 100.f);
	// Character mesh setup
	CharacterMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = CharacterMesh;
	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AEPawn::GrowBox()
{
	if (CharacterMesh->GetRelativeScale3D().X < ActorMaxSize && CharacterMesh->GetRelativeScale3D().Y < ActorMaxSize)
	{
		CharacterMesh->SetRelativeScale3D(CharacterMesh->GetRelativeScale3D() + FVector(GrowStep, GrowStep, 0));
		Mass += GrowStep;
	}
}
void AEPawn::ShrinkBox()
{
	if (CharacterMesh->GetRelativeScale3D().X > ActorMinSize && CharacterMesh->GetRelativeScale3D().Y > ActorMinSize)
	{
		CharacterMesh->SetRelativeScale3D(CharacterMesh->GetRelativeScale3D() - FVector(GrowStep, GrowStep, 0));
		Mass -= GrowStep;
	}
}

void AEPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	CharacterMesh->SetRelativeScale3D(FVector(ActorMinSize, ActorMinSize, 1));
	this->TeleportTo(StartLocation, FRotator(0,0,0));
}

