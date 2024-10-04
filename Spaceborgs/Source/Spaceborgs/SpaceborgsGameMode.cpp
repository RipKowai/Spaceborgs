// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceborgsGameMode.h"
#include "SpaceborgsCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASpaceborgsGameMode::ASpaceborgsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
