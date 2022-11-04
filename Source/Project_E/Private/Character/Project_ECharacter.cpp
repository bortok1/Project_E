// Copyright Bean Bin, All Rights Reserved.

#include "Character/Project_ECharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

AProject_ECharacter::AProject_ECharacter() :
	CollisionBox(nullptr),
	CharacterMeshMinSize(2.f),
	CharacterMeshMaxSize(8.f),
	GrowStep(1.f)	
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Character mesh setup
	CharacterMesh = this->FindComponentByClass<USkeletalMeshComponent>();
	CharacterMesh->SetRelativeScale3D(FVector(CharacterMeshMinSize, CharacterMeshMinSize, CharacterMeshMinSize));

	// Collision setup
	CollisionBox = this->FindComponentByClass<UBoxComponent>();

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AProject_ECharacter::OnHit);
	

}

void AProject_ECharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

}

void AProject_ECharacter::GrowOnStop()
{
	if (CharacterMesh->GetRelativeScale3D().X <= CharacterMeshMaxSize && CharacterMesh->GetRelativeScale3D().Y <= CharacterMeshMaxSize && CharacterMesh->GetRelativeScale3D().Z <= CharacterMeshMaxSize)
	{
		CharacterMesh->SetRelativeScale3D(CharacterMesh->GetRelativeScale3D() + FVector(GrowStep, GrowStep, GrowStep));
		if(CollisionBox  != nullptr)
			CollisionBox->SetBoxExtent(CharacterMesh->GetRelativeScale3D());
	}
}

void AProject_ECharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HitComponent->GetOwner()->SetActorLocation(location);
	CharacterMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	if (CollisionBox != nullptr) {
		CollisionBox->SetBoxExtent(CharacterMesh->GetRelativeScale3D());
	}
}

void AProject_ECharacter::BeginPlay()
{
	Super::BeginPlay();
	location = GetCapsuleComponent()->GetComponentLocation();
}