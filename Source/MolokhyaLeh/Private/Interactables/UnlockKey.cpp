
#include "Interactables/UnlockKey.h"
#include "Inventory/Inventory.h"
#include "Character/MlCharacter.h"

AUnlockKey::AUnlockKey()
{
	PrimaryActorTick.bCanEverTick = true;

}
void AUnlockKey::BeginPlay()
{
	Super::BeginPlay();
	
}

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