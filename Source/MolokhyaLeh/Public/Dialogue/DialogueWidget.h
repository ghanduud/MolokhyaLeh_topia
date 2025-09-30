#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dialogue/Dialogue.h"
#include "DialogueWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class UButton;
class UImage; 
class UTextBlock;


DECLARE_DELEGATE(FDialogueAdvance);
DECLARE_DELEGATE_OneParam(FDialogueChoose, int32 /*ChoiceIndex*/);

UCLASS()
class MOLOKHYALEH_API UDialogueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // BindWidget: create a BP (WBP_Dialogue) with these names
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* Text_Dialogue;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UVerticalBox* Box_Choices;

    // Speed: one word per this many seconds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    float WordInterval = 0.04f;

    UPROPERTY(meta = (BindWidgetOptional)) UImage* NPCPortraitImage;
    UPROPERTY(meta = (BindWidgetOptional)) UTextBlock* NPCNameText;

    // Track current node id (so BP knows where we are)
    UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
    FString CurrentNodeId;

    // Haggle price we’re building toward
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue|Trade")
    int32 HagglePrice = 60;          // default/base price

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue|Trade")
    FName CoinId = "Coin";

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue|Trade")
    FName SilkId = "Silk";

    // Called by component
    void ShowParagraph(const FText& Paragraph, const TArray<struct FDialogueChoice>& Choices);

    bool IsParagraphFullyShown() const { return bFullyRevealed; }
    bool HasChoices() const { return PendingChoices.Num() > 0; }
    void RevealAll();

    // Delegates back to component
    FDialogueAdvance OnAdvance;
    FDialogueChoose  OnChoose;

    UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
    void BP_BuildChoices(const TArray<FDialogueChoice>& Choices);

    UFUNCTION(BlueprintCallable) void UI_RevealAll();
    UFUNCTION(BlueprintCallable) void UI_ChooseIndex(int32 Index);  
    UFUNCTION(BlueprintCallable) void ApplySpeaker(UTexture2D* Portrait, const FText& Name);
    // BP can set/adjust price from choice rows
    UFUNCTION(BlueprintCallable, Category = "Dialogue|Trade")
    void UI_SetPrice(int32 NewPrice) { HagglePrice = NewPrice; }

    UFUNCTION(BlueprintCallable, Category = "Dialogue|Trade")
    void UI_AddPrice(int32 Delta) { HagglePrice += Delta; }

    // Try to buy silk; returns true on success (coins removed, silk added)
    UFUNCTION(BlueprintCallable, Category = "Dialogue|Trade")
    bool UI_TryBuySilk();

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
