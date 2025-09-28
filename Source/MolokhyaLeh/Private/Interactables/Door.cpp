// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Door.h"
#include "Character/MlCharacter.h"
#include "Inventory/Inventory.h"


// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::Interact_Implementation(AMlCharacter* Interactor)
{
	IInteractable::Interact_Implementation(Interactor);
	
	if (bIsLocked)
	{
		bool bHasKey = false;

		if (Interactor && Interactor->Inventory && Interactor->Inventory->HasItem(RequiredKeyId))
		{
			if (bConsumeKeyOnUse)
				Interactor->Inventory->ConsumeItem(RequiredKeyId, 1);

			bHasKey = true;
		}

		if (bHasKey)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player has the key! Unlocking door."));
			bIsLocked = false;
			OpenDoor();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Door is locked. Player needs a key."));
			return;
		}
	}
	else
	{
		if (bIsOpen)
		{
			CloseDoor();
		}
		else
		{
			OpenDoor();
		}
	}
}

void ADoor::OpenDoor()
{
	// TODO: add animation / sound
	bIsOpen = true;
	UE_LOG(LogTemp, Warning, TEXT("Door opened"));
}

void ADoor::CloseDoor()
{
	// TODO: add animation / sound
	bIsOpen = false;
	UE_LOG(LogTemp, Warning, TEXT("Door closed"));
}