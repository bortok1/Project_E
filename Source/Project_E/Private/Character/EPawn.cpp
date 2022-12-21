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

}

void AEPawn::BeginPlay()
{
	Super::BeginPlay();

	bStopMeNow = false;
	ResetTimer();

	StartPosition = GetActorLocation();
}


void AEPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AEPlayerController* EPC = Cast<AEPlayerController>(Controller);
	check(EIC && EPC);
	EIC->BindAction(EPC->MoveAction, ETriggerEvent::Triggered, this, &AEPawn::Move);

	const ULocalPlayer* LocalPlayer = EPC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(EPC->PawnMappingContext, 0);
}

void AEPawn::Win()
{
	WriteScoreTimer();
	Die();
}

void AEPawn::Die()
{
	EDiedEvent();
	OnDeath.Broadcast();
	SizeComponent->SetDefaultSize();
	Camera->SetDefaultFieldOfView();
	bStopMeNow = true;
	FVector loc = GetActorLocation();
	FRotator rot = GetActorRotation();
	SpawnObject(loc, rot);
	SetActorLocation(StartPosition);
	StopTimer();
	ResetTimer();
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

void AEPawn::Move(const struct FInputActionValue& ActionValue)
{
	FVector offset(0, 0, 0);
	FRotator rotation(GetActorRotation().Pitch, GetActorRotation().Yaw, 0);
	if (GetActorScale().X == 1) {
		offset = FVector(0, 0, 0);
	}
	else if (GetActorScale().X == 2) {
		offset = FVector(-100, -100, 0);
	}
	else if (GetActorScale().X == 3) {
		offset = FVector(-200, -200, 0);
	}
	else if (GetActorScale().X == 4) {
		offset = FVector(-250, -250, 0);
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Particles, GetActorLocation() + (offset * GetActorRotation().Vector()), -1 * GetActorRotation());
	const FVector VectorToCursor = MovementComponent->GetVectorTowardsCursor(GetMousePosition());
	AddMovementInput(VectorToCursor, MovementComponent->MoveScale/SizeComponent->GetMass());
	CharacterMesh->SetRelativeRotation(VectorToCursor.Rotation());
}

FVector2D AEPawn::GetMousePosition() const
{
	FVector2D Mouse = FVector2d::Zero();
	EPlayerController->GetMousePosition(Mouse.X, Mouse.Y);
	return Mouse;
}

void AEPawn::SpawnObject(FVector loc, FRotator rot) 
{
	FActorSpawnParameters SpawnParams;
	AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(ActorToSpawn, loc, rot, SpawnParams);
}
