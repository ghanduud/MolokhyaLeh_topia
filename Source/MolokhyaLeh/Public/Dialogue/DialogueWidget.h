#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dialogue/Dialogue.h"
#include "DialogueWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class UButton;

DECLARE_DELEGATE(FDialogueAdvance);
DECLARE_DELEGATE_OneParam(FDialogueChoose, int32 /*ChoiceIndex*/);

UCLASS()
class MOLOKHYALEH_API UDialogueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // BindWidget: create a BP (WBP_Dialogue) with these names
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Dialogue;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* Box_Choices;

    // Speed: one word per this many seconds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    float WordInterval = 0.04f;

    // Called by component
    void ShowParagraph(const FText& Paragraph, const TArray<struct FDialogueChoice>& Choices);

    bool IsParagraphFullyShown() const { return bFullyRevealed; }
    bool HasChoices() const { return PendingChoices.Num() > 0; }
    void RevealAll();

    // Delegates back to component
    FDialogueAdvance OnAdvance;
    FDialogueChoose  OnChoose;

protected:
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
    void BeginType(const FString& Full);
    void TickType();

    void BuildChoices();

private:
    FTimerHandle TypeTimer;
    TArray<FString> Words;
    int32 WordIndex = 0;
    FString CurrentShown;
    bool bFullyRevealed = true;

    TArray<FDialogueChoice> PendingChoices;
};
