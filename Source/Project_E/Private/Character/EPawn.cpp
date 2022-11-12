// Copyright Bean Bin, All Rights Reserved.


#include "Character/EPawn.h"
#include "Character/PlayerController/EPlayerController.h"
#include "PickUp.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Subsystems/PanelExtensionSubsystem.h>
#include <Misc/OutputDeviceNull.h>

// Sets default values
AEPawn::AEPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>("CharacterMesh");
	SetRootComponent(CharacterMesh);

	CharacterMesh->OnComponentBeginOverlap.AddDynamic(this, &AEPawn::OnOverlap);
	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	TSubclassOf<UUserWidget> TimerWidgetClass;
	ConstructorHelpers::FClassFinder<UUserWidget> BP_TimerWidgetClass(TEXT("/Game/Characters/BP_TimerWidget"));
	
	if (BP_TimerWidgetClass.Class) { 
		TimerWidgetClass = BP_TimerWidgetClass.Class;
	}
	TimerWidgetRef = CreateWidget<UUserWidget>(GetWorld(), TimerWidgetClass);
	
}

bool AEPawn::GrowBox()
{
	if (CharacterMesh->GetRelativeScale3D().X < ActorMaxSize && CharacterMesh->GetRelativeScale3D().Y < ActorMaxSize)
	{
		CharacterMesh->SetRelativeScale3D(CharacterMesh->GetRelativeScale3D() + FVector(GrowStep, GrowStep, 0));
		Mass += GrowStep;
		TopDownCameraComponent->FieldOfView += FOVStep;
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
		TopDownCameraComponent->FieldOfView -= FOVStep;
		return true;
	}
	return false;
}

bool AEPawn::ResetTimer()
{
	if (TimerWidgetRef) {
		FOutputDeviceNull er;
		const TCHAR* command = TEXT("Reset");
		TimerWidgetRef->CallFunctionByNameWithArguments(command, er, nullptr, true);
		return true;
	}
	return false;
}

bool AEPawn::StopTimer()
{
	if (TimerWidgetRef) {
		FOutputDeviceNull er;
		const TCHAR* command = TEXT("Stop");
		TimerWidgetRef->CallFunctionByNameWithArguments(command, er, nullptr, true);
		return true;
	}
	return false;
}

bool AEPawn::WriteScoreTimer()
{
	if (TimerWidgetRef) {
		FOutputDeviceNull er;
		const TCHAR* command = TEXT("WriteScore");
		TimerWidgetRef->CallFunctionByNameWithArguments(command, er, nullptr, true);
		return true;
	}
	return false;
}

void AEPawn::OnHit(FVector StartLocation)
{
	
	CharacterMesh->SetRelativeScale3D(FVector(ActorMinSize, ActorMinSize, 1));
	Mass = DefaultMass;
	TopDownCameraComponent->FieldOfView = DefaultFOV;
	//
	this->TeleportTo(StartLocation, FRotator(0, 90, 0));
}

void AEPawn::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: Nie dziala, trzeba zrobic zamiast blueprintu
	//APickUp* pickUp = Cast<APickUp>(OtherActor);
	//if (pickUp)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Hit");
}

