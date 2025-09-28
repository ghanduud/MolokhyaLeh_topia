#include "Character/NPCCharacter.h"
#include "Dialogue/Dialogue.h"
#include "Character/MlCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"

ANPCCharacter::ANPCCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    Dialogue = CreateDefaultSubobject<UDialogue>(TEXT("Dialogue"));
}

void ANPCCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (Dialogue && !Dialogue->DialogueFile.IsEmpty())
    {
        Dialogue->LoadFromTextFile();
    }
}

void ANPCCharacter::Interact_Implementation(AMlCharacter* Interactor)
{
    if (!Dialogue || Dialogue->IsRunning()) return;

    APlayerController* PC = Interactor ? Cast<APlayerController>(Interactor->GetController()) : nullptr;
    if (PC)
    {
        // Ensure DialogueWidgetClass is set on the component (e.g., WBP_Dialogue)
        Dialogue->StartDialogue(PC);
    }
}
