// Copyright Epic Games, Inc. All Rights Reserved.

#include "CastleInquisitorsGameMode.h"
#include "CastleInquisitorsCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACastleInquisitorsGameMode::ACastleInquisitorsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Gameplay/Blueprints/BP_CI_Knight_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
