// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/UnlockKey.h"
#include "Inventory/Inventory.h"
#include "Character/MlCharacter.h"
#include "Interactables/UnlockKey.h"

// Sets default values
AUnlockKey::AUnlockKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnlockKey::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnlockKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnlockKey::Interact_Implementation(AMlCharacter* Interactor)
{
    if (Interactor && Interactor->Inventory)
    {
        Interactor->Inventory->AddItem(KeyId, ItemType, Quantity, KeyDisplayName);
        Destroy();
    }
}