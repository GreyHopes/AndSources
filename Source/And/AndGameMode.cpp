// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AndGameMode.h"
#include "AndHUD.h"
#include "AndCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAndGameMode::AAndGameMode(): Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/And/Blueprints/BP_MainCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAndHUD::StaticClass();

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassFinder(TEXT("/Game/And/Blueprints/PlayerController"));
	PlayerControllerClass = PlayerControllerClassFinder.Class;
}



