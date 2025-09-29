#include "Dialogue/DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Dialogue/Dialogue.h" 

void UDialogueWidget::ShowParagraph(const FText& Paragraph, const TArray<FDialogueChoice>& Choices)
{
    PendingChoices = Choices;

    GetWorld()->GetTimerManager().ClearTimer(TypeTimer);
    CurrentShown.Reset();
    WordIndex = 0;
    bFullyRevealed = false;

    // Clear UI
    if (Box_Choices) Box_Choices->ClearChildren();
    if (Text_Dialogue) Text_Dialogue->SetText(FText::GetEmpty());

    BeginType(Paragraph.ToString());
}

void UDialogueWidget::BeginType(const FString& Full)
{
    Words.Empty();
    Full.ParseIntoArrayWS(Words); // split by whitespace

    if (Words.Num() == 0)
    {
        bFullyRevealed = true;
        if (PendingChoices.Num() > 0)
        {
            BP_BuildChoices(PendingChoices);   // let Blueprint create the buttons
        }
        return;
    }

    // Tick words
    GetWorld()->GetTimerManager().SetTimer(
        TypeTimer, this, &UDialogueWidget::TickType, WordInterval, true);
}

void UDialogueWidget::TickType()
{
    if (!Text_Dialogue) return;

    if (WordIndex < Words.Num())
    {
        if (!CurrentShown.IsEmpty()) CurrentShown.AppendChar(' ');
        CurrentShown += Words[WordIndex++];
        Text_Dialogue->SetText(FText::FromString(CurrentShown));
    }
    else
    {
        // finished
        GetWorld()->GetTimerManager().ClearTimer(TypeTimer);
        bFullyRevealed = true;
        if (PendingChoices.Num() > 0)
        {
            BP_BuildChoices(PendingChoices);   // let Blueprint create the buttons
        }
    }
}


void UDialogueWidget::RevealAll()
{
    if (!Text_Dialogue) return;
    GetWorld()->GetTimerManager().ClearTimer(TypeTimer);

    CurrentShown = FString::Join(Words, TEXT(" "));
    Text_Dialogue->SetText(FText::FromString(CurrentShown));
    bFullyRevealed = true;

    if (PendingChoices.Num() > 0)
    {
        BP_BuildChoices(PendingChoices);
    }
}

void UDialogueWidget::BuildChoices()
{
    if (!Box_Choices) return;

    Box_Choices->ClearChildren();
    if (PendingChoices.Num() == 0) return;

    for (int32 i = 0; i < PendingChoices.Num(); ++i)
    {
        UButton* Btn = NewObject<UButton>(this);
        UTextBlock* Label = NewObject<UTextBlock>(this);
        Label->SetText(PendingChoices[i].Text);
        Btn->AddChild(Label);

        const int32 ChoiceIndex = i;


        Box_Choices->AddChild(Btn);
    }
}

FReply UDialogueWidget::NativeOnKeyDown(const FGeometry& G, const FKeyEvent& E)
{
    if (E.GetKey() == EKeys::SpaceBar)
    {
        if (!bFullyRevealed) RevealAll();
        else if (OnAdvance.IsBound()) OnAdvance.Execute();
        return FReply::Handled();
    }

    // 1..9 selects choices[0..8]
    if (PendingChoices.Num() > 0)
    {
        const TMap<FKey, int32> KeyToIdx = {
            {EKeys::One,0},{EKeys::Two,1},{EKeys::Three,2},
            {EKeys::Four,3},{EKeys::Five,4},{EKeys::Six,5},
            {EKeys::Seven,6},{EKeys::Eight,7},{EKeys::Nine,8}
        };
        if (const int32* Idx = KeyToIdx.Find(E.GetKey()))
        {
            if (*Idx < PendingChoices.Num())
            {
                if (!bFullyRevealed) RevealAll();
                if (OnChoose.IsBound()) OnChoose.Execute(*Idx);
                return FReply::Handled();
            }
        }
    }
    return Super::NativeOnKeyDown(G, E);
}

void UDialogueWidget::UI_RevealAll()
{
    RevealAll();
}

void UDialogueWidget::UI_ChooseIndex(int32 Index)
{
    if (!bFullyRevealed)
    {
        RevealAll();
    }
    if (OnChoose.IsBound())
    {
        OnChoose.Execute(Index);
    }
}