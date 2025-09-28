#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Interactable.h"
#include "NPCCharacter.generated.h"

class UDialogue;
class AMlCharacter;

UCLASS()
class MOLOKHYALEH_API ANPCCharacter : public ACharacter, public IInteractable
{
    GENERATED_BODY()

public:
    ANPCCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    UDialogue* Dialogue;

protected:
    virtual void BeginPlay() override;

public:
    // Interactable implementation
    virtual void Interact_Implementation(AMlCharacter* Interactor) override;
};
