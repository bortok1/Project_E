// Copyright Bean Bin, All Rights Reserved.


#include "Character/GameMode/EGameMode.h"

#include "Character/EPawn.h"
#include "Character/PlayerController/EPlayerController.h"

AEGameMode::AEGameMode()
{
	//tell your custom game mode to use your custom player controller
	PlayerControllerClass = AEPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/BP_EPawn"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		DefaultPawnClass = AEPawn::StaticClass();
	}
}
