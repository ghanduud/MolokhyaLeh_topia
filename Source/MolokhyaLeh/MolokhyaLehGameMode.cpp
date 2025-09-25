// Copyright Epic Games, Inc. All Rights Reserved.

#include "MolokhyaLehGameMode.h"
#include "MolokhyaLehCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMolokhyaLehGameMode::AMolokhyaLehGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
