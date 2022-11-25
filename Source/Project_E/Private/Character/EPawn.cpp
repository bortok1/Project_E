// Copyright Bean Bin, All Rights Reserved.


#include "Character/EPawn.h"
#include "Character/PlayerController/EPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Subsystems/PanelExtensionSubsystem.h>
#include <Misc/OutputDeviceNull.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"


// Sets default values
AEPawn::AEPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>("CharacterMesh");
	SetRootComponent(CharacterMesh);

	CharacterMesh->OnComponentHit.AddDynamic(this, &AEPawn::NotifyHit);
	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, -90.f, 0.f));
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

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>("Movement");
	
	MoveScale = 1.f;
}

void AEPawn::BeginPlay()
{
	Super::BeginPlay();

	EPlayerController = Cast<AEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AEPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AEPlayerController* EPC = Cast<AEPlayerController>(Controller);
	check(EIC && EPC);
	EIC->BindAction(EPC->MoveAction, ETriggerEvent::Triggered, this, &AEPawn::Move);

	ULocalPlayer* LocalPlayer = EPC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(EPC->PawnMappingContext, 0);
}

void AEPawn::Move(const struct FInputActionValue& ActionValue)
{
	// Take half of screen resolution
	FVector2D HalfResolution = FVector2D::Zero();
	if ( GEngine && GEngine->GameViewport )
	{
		GEngine->GameViewport->GetViewportSize(HalfResolution);
		HalfResolution *= 0.5f;
	}

	// Location of mouse cursor on 2D screen
	FVector2D CursorLocation; EPlayerController->GetMousePosition(CursorLocation.X, CursorLocation.Y);

	// Direction from screen center (player) to mouse cursor
	 const FVector WorldDirectionVector3d = FVector((CursorLocation - HalfResolution).GetSafeNormal(), 0);
	 const FRotator Rotation = (GetActorRotation() * (AngularDumping - 1) + WorldDirectionVector3d.Rotation()) * (1/AngularDumping);
	
	// TODO: Player can't make full revolution
	// if (abs(Rotation.Yaw) - abs(GetActorRotation().Yaw) > 90)
	{
		
	}
	
	AddMovementInput(WorldDirectionVector3d, MoveScale);
	SetActorRotation(Rotation);
}

void AEPawn::NotifyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// CharacterMesh->SetRelativeScale3D(FVector(ActorMinSize, ActorMinSize, 1));
	// Mass = DefaultMass;
	// TopDownCameraComponent->FieldOfView = DefaultFOV;
	// OnDeath.Broadcast();
	// this->TeleportTo(StartLocation, FRotator(0, 90, 0));
}

void AEPawn::GrowBox()
{
	if (CharacterMesh->GetRelativeScale3D().X < ActorMaxSize && CharacterMesh->GetRelativeScale3D().Y < ActorMaxSize)
	{
		CharacterMesh->SetRelativeScale3D(CharacterMesh->GetRelativeScale3D() + FVector(GrowStep, GrowStep, 0));
		Mass += GrowStep;
		TopDownCameraComponent->FieldOfView += FOVStep;
	}
}

void AEPawn::ShrinkBox()
{
	if (CharacterMesh->GetRelativeScale3D().X > ActorMinSize && CharacterMesh->GetRelativeScale3D().Y > ActorMinSize)
	{
		CharacterMesh->SetRelativeScale3D(CharacterMesh->GetRelativeScale3D() - FVector(GrowStep, GrowStep, 0));
		Mass -= GrowStep;
		TopDownCameraComponent->FieldOfView -= FOVStep;
	}
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
