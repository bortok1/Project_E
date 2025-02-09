// Copyright Bean Bin, All Rights Reserved.


#include "Character/EPawn.h"
#include <Misc/OutputDeviceNull.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Character/Components/ECameraComponent.h"
#include "Character/Components/EFloatingPawnMovement.h"
#include "Character/Components/SizeManagerComponent.h"
#include "Character/PlayerController/EPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AEPawn::AEPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Mesh
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>("CharacterMesh");
	SetRootComponent(CharacterMesh);

	CharacterMesh->OnComponentHit.AddDynamic(this, &AEPawn::OnActorHit);

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, -90.f, 0.f));
	CameraBoom->bDoCollisionTest = true; // Want to pull camera in when it collides with level

	// Create a camera
	Camera = CreateDefaultSubobject<UECameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	EPlayerController = Cast<AEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	MovementComponent = CreateDefaultSubobject<UEFloatingPawnMovement>("MovementComponent");
	SizeComponent = CreateDefaultSubobject<USizeManagerComponent>("SizeComponent");
	
	TSubclassOf<UUserWidget> TimerWidgetClass;
	ConstructorHelpers::FClassFinder<UUserWidget> BP_TimerWidgetClass(TEXT("/Game/Characters/BP_TimerWidget"));
	
	if (BP_TimerWidgetClass.Class) { 
		TimerWidgetClass = BP_TimerWidgetClass.Class;
	}
	
	TimerWidgetRef = CreateWidget<UUserWidget>(GetWorld(), TimerWidgetClass);

	isAnimationPlaying = false;
	bMoveInputPressed = false;
}

void AEPawn::BeginPlay()
{
	Super::BeginPlay();

	StartPosition = GetActorLocation();
}

void AEPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AEPlayerController* EPC = Cast<AEPlayerController>(Controller);
	check(EIC && EPC);
	EIC->BindAction(EPC->MoveAction, ETriggerEvent::Triggered, this, &AEPawn::MoveEvent);

	const ULocalPlayer* LocalPlayer = EPC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(EPC->PawnMappingContext, 0);
}

void AEPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovementComponent->Move(DeltaTime, bMoveInputPressed);
	bMoveInputPressed = false;
}

void AEPawn::Win()
{
	if(isAnimationPlaying)
		return;
	WriteScoreTimer();
	EWinEvent();
}

void AEPawn::Die()
{
	if (isAnimationPlaying)
		return;
	
	EDiedEvent();
	
	// Spawn Death mark
	SpawnDeathMark(GetActorLocation(), GetActorRotation());

	Camera->Shake();
	
	ResetLevel();
}

void AEPawn::ResetLevel()
{
	isAnimationPlaying = true;
	// Wait for animations
	FTimerDelegate Delegate;
	Delegate.BindLambda([&]()
		{
			TeleportTo(StartPosition, StartRotation, false, true);
			//TODO tu coś się psuje
			EAnimationsDoneEvent();
			SizeComponent->SetDefaultSize();
	
			Camera->SetDefaultFieldOfView();
				
			OnDeath.Broadcast();
			// Score Timer
			StopTimer();
			isAnimationPlaying = false;
		});

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, 0.7f, false);
}

void AEPawn::OnActorHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                        UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult)
{
	if(Actor->GetActorLocation().Z + 100.f > GetActorLocation().Z)
	{
		Die();
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

void AEPawn::MoveEvent(const struct FInputActionValue& ActionValue)
{
	JoystickAxis = ActionValue.Get<FVector2D>();

	const FVector RootBackwardVector = -RootComponent->GetForwardVector();
	const FRotator RootRotation = RootBackwardVector.Rotation();
	FVector ParticlePositionOffset;

	switch (static_cast<int>(GetActorScale().X)) {
	case 1:
		ParticlePositionOffset = FVector(0, 0, 0);
		break;
	case 2:
		ParticlePositionOffset = FVector(50, 50, 0);
		break;
	case 3:
		ParticlePositionOffset = FVector(100, 100, 0);
		break;
	default:
		ParticlePositionOffset = FVector(150, 150, 0);
	}
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Particles,
		GetActorLocation() + ParticlePositionOffset * RootBackwardVector, RootRotation, FVector(GetActorScale().X));

	bMoveInputPressed = true;
}

void AEPawn::SpawnDeathMark(FVector loc, FRotator rot) const
{
	GetWorld()->SpawnActor<AActor>(ADeathMark, loc, rot);
}
