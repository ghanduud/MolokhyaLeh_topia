#include "Dialogue/DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

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
        BuildChoices();
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
        BuildChoices();
    }
}

void UDialogueWidget::RevealAll()
{
    if (!Text_Dialogue) return;
    GetWorld()->GetTimerManager().ClearTimer(TypeTimer);

    CurrentShown = FString::Join(Words, TEXT(" "));
    Text_Dialogue->SetText(FText::FromString(CurrentShown));
    bFullyRevealed = true;

    BuildChoices();
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

        Btn->OnClicked.AddUniqueDynamic(this, &UDialogueWidget::RevealAll); // ensure full text
        const int32 ChoiceIndex = i;
        Btn->OnClicked.AddLambda([this, ChoiceIndex]()
            {
                if (OnChoose.IsBound())
                {
                    OnChoose.Execute(ChoiceIndex);
                }
            });

        Box_Choices->AddChild(Btn);
    }
}

FReply UDialogueWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::SpaceBar)
    {
        if (!bFullyRevealed) RevealAll();
        else if (OnAdvance.IsBound()) OnAdvance.Execute();
        return FReply::Handled();
    }
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
