#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dialogue.generated.h"

class APlayerController;
class UDialogueWidget;

USTRUCT(BlueprintType)
struct FDialogueChoice
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Text;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NextId;
};

USTRUCT(BlueprintType)
struct FDialogueNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Paragraph;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDialogueChoice> Choices;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MOLOKHYALEH_API UDialogue : public UActorComponent
{
    GENERATED_BODY()

public:
    UDialogue();

    // Text file path relative to Project Content/Dialogue/, e.g., "Npc01.txt"
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    FString DialogueFile;

    // Default widget class (create a BP subclass of UDialogueWidget and assign it)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    TSubclassOf<UUserWidget> DialogueWidgetClass;

    // Load & parse the text file (call once in BeginPlay or on first interact)
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    bool LoadFromTextFile();

    // Begin dialogue (spawns widget, switches to UI input mode)
    void StartDialogue(APlayerController* PC);

    // Called by Space/skip (widget forwards to component)
    void AdvanceOrSkip();

    // Called when a choice is clicked in the widget
    void Choose(int32 ChoiceIndex);

    // For NPCs to know if a dialogue is running
    bool IsRunning() const { return bRunning; }

protected:
    virtual void BeginPlay() override;

private:
    // Parsing helpers
    bool Parse(const FString& Raw);
    void ShowNode(const FString& NodeId);
    void EndDialogue();

private:
    UPROPERTY()
    UDialogueWidget* ActiveWidget = nullptr;

    UPROPERTY()
    APlayerController* OwnerPC = nullptr;

    UPROPERTY()
    TMap<FString, FDialogueNode> Nodes;

    UPROPERTY()
    FString CurrentId;

    bool bRunning = false;
};
