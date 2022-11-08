// Copyright Bean Bin, All Rights Reserved.


#include "Character/EPawn.h"
#include "PickUp.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AEPawn::AEPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	StartLocation = FVector(1200.f, 1500.f, 100.f);
	
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>("CharacterMesh");
	SetRootComponent(CharacterMesh);

	CharacterMesh->OnComponentBeginOverlap.AddDynamic(this, &AEPawn::OnOverlap);
	
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

bool AEPawn::GrowBox()
{
	if (CharacterMesh->GetRelativeScale3D().X < ActorMaxSize && CharacterMesh->GetRelativeScale3D().Y < ActorMaxSize)
	{
		CharacterMesh->SetRelativeScale3D(CharacterMesh->GetRelativeScale3D() + FVector(GrowStep, GrowStep, 0));
		Mass += GrowStep;
		return true;
	}
	return false;
}
bool AEPawn::ShrinkBox()
{
	if (CharacterMesh->GetRelativeScale3D().X > ActorMinSize && CharacterMesh->GetRelativeScale3D().Y > ActorMinSize)
	{
		CharacterMesh->SetRelativeScale3D(CharacterMesh->GetRelativeScale3D() - FVector(GrowStep, GrowStep, 0));
		Mass -= GrowStep;
		return true;
	}
	return false;
}

void AEPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	CharacterMesh->SetRelativeScale3D(FVector(ActorMinSize, ActorMinSize, 1));
	this->TeleportTo(StartLocation, FRotator(0,0,0));
}

void AEPawn::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Todo Nie dziala, trzeba zrobiæ zamiast blueprintu
	//APickUp* pickUp = Cast<APickUp>(OtherActor);
	//if (pickUp)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Hit");
}

