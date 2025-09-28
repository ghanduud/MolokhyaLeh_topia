#include "Dialogue/Dialogue.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dialogue/DialogueWidget.h"

UDialogue::UDialogue()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDialogue::BeginPlay()
{
    Super::BeginPlay();
}

bool UDialogue::LoadFromTextFile()
{
    if (DialogueFile.IsEmpty()) return false;

    // NOTE: For packaged builds, this won't find files in Content.
    // For shipping, convert to DataTable/PrimaryDataAsset.
    const FString Path = FPaths::ProjectContentDir() / TEXT("Dialogue") / DialogueFile;

    FString Raw;
    if (!FFileHelper::LoadFileToString(Raw, *Path))
    {
        UE_LOG(LogTemp, Error, TEXT("Dialogue file not found: %s"), *Path);
        return false;
    }
    Nodes.Empty();
    return Parse(Raw);
}

bool UDialogue::Parse(const FString& Raw)
{
    // Minimal parser for [NODE] blocks, choices start with "> " and contain "->"
    TArray<FString> Lines;
    Raw.ParseIntoArrayLines(Lines);

    FString CurrentNodeId;
    FString ParaAccum;
    TArray<FDialogueChoice> ChoiceAccum;

    auto FlushNode = [&]()
        {
            if (CurrentNodeId.IsEmpty()) return;
            FDialogueNode Node;
            Node.Id = CurrentNodeId;
            Node.Paragraph = FText::FromString(ParaAccum.TrimStartAndEnd());
            Node.Choices = ChoiceAccum;
            Nodes.Add(CurrentNodeId, MoveTemp(Node));
            ParaAccum.Reset();
            ChoiceAccum.Reset();
        };

    for (const FString& L : Lines)
    {
        FString Line = L;

        // Node header: [ID]
        if (Line.StartsWith(TEXT("[")) && Line.EndsWith(TEXT("]")))
        {
            FlushNode();
            CurrentNodeId = Line.Mid(1, Line.Len() - 2).TrimStartAndEnd();
            continue;
        }

        // Blank line breaks paragraph; keep as newline
        if (Line.TrimStartAndEnd().IsEmpty())
        {
            ParaAccum += TEXT("\n");
            continue;
        }

        // Choice line: "> text -> NEXT"
        if (Line.StartsWith(TEXT(">")))
        {
            FString Work = Line.Mid(1).TrimStartAndEnd();
            FString Left, Right;
            if (Work.Split(TEXT("->"), &Left, &Right))
            {
                FDialogueChoice C;
                C.Text = FText::FromString(Left.TrimEnd());
                C.NextId = Right.TrimStartAndEnd();
                ChoiceAccum.Add(MoveTemp(C));
            }
            continue;
        }

        // Regular paragraph line
        if (!ParaAccum.IsEmpty()) ParaAccum += TEXT(" ");
        ParaAccum += Line;
    }
    FlushNode();

    if (!Nodes.Contains(TEXT("START")))
    {
        UE_LOG(LogTemp, Error, TEXT("Dialogue missing [START] node"));
        return false;
    }
    return true;
}

void UDialogue::StartDialogue(APlayerController* PC)
{
    if (!PC || !DialogueWidgetClass) return;
    OwnerPC = PC;

    if (!ActiveWidget)
    {
        ActiveWidget = CreateWidget<UDialogueWidget>(PC, DialogueWidgetClass);
    }
    if (!ActiveWidget) return;

    ActiveWidget->AddToViewport(10000);
    // Give widget the callbacks & data
    ActiveWidget->OnAdvance.BindUObject(this, &UDialogue::AdvanceOrSkip);
    ActiveWidget->OnChoose.BindUObject(this, &UDialogue::Choose);

    // Input mode UI Only
    FInputModeUIOnly Mode;
    Mode.SetWidgetToFocus(ActiveWidget->TakeWidget());
    Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(Mode);
    PC->bShowMouseCursor = true;

    bRunning = true;
    CurrentId = TEXT("START");
    ShowNode(CurrentId);
}

void UDialogue::ShowNode(const FString& NodeId)
{
    const FDialogueNode* Node = Nodes.Find(NodeId);
    if (!Node || !ActiveWidget) { EndDialogue(); return; }

    ActiveWidget->ShowParagraph(Node->Paragraph, Node->Choices);
    CurrentId = NodeId;

    // If there are no choices and no text, end
    if (Node->Paragraph.IsEmpty() && Node->Choices.Num() == 0)
    {
        EndDialogue();
    }
}

void UDialogue::AdvanceOrSkip()
{
    if (!ActiveWidget) return;

    if (!ActiveWidget->IsParagraphFullyShown())
    {
        ActiveWidget->RevealAll();
    }
    else
    {
        // If choices exist, widget shows them and waits for Choose()
        if (!ActiveWidget->HasChoices())
        {
            // sequential: try to go to an implicit NEXT if named "NEXT"
            const FDialogueNode* Node = Nodes.Find(CurrentId);
            if (Node && Node->Choices.Num() == 1 && Node->Choices[0].NextId.Equals(TEXT("NEXT"), ESearchCase::IgnoreCase))
            {
                ShowNode(Node->Choices[0].NextId);
            }
            else
            {
                // No choices, try END
                EndDialogue();
            }
        }
    }
}

void UDialogue::Choose(int32 ChoiceIndex)
{
    const FDialogueNode* Node = Nodes.Find(CurrentId);
    if (!Node || !Node->Choices.IsValidIndex(ChoiceIndex)) return;

    const FString Next = Node->Choices[ChoiceIndex].NextId;
    if (Next.Equals(TEXT("END"), ESearchCase::IgnoreCase))
    {
        EndDialogue();
    }
    else
    {
        ShowNode(Next);
    }
}

void UDialogue::EndDialogue()
{
    bRunning = false;

    if (OwnerPC)
    {
        FInputModeGameOnly Mode;
        OwnerPC->SetInputMode(Mode);
        OwnerPC->bShowMouseCursor = false;
    }

    if (ActiveWidget)
    {
        ActiveWidget->RemoveFromParent();
        ActiveWidget = nullptr;
    }
}
