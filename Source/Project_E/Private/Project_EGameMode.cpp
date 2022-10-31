// Copyright Bean Bin, All Rights Reserved.

#include "Project_EGameMode.h"
#include "Project_EPlayerController.h"
#include "Character/Project_ECharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_EGameMode::AProject_EGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AProject_EPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}