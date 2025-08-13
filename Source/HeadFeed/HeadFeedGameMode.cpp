// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeadFeedGameMode.h"
#include "HeadFeedCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHeadFeedGameMode::AHeadFeedGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
