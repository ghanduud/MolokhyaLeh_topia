#include "Character/GuardCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Character/MlCharacter.h"
#include "Components/CapsuleComponent.h"

AGuardCharacter::AGuardCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    // Sight settings (tweak in Details later)
    SightConfig->SightRadius = 2000.f;
    SightConfig->LoseSightRadius = 2200.f;
    SightConfig->PeripheralVisionAngleDegrees = 70.f;   // 140° cone
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 0.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    Perception->ConfigureSense(*SightConfig);
    Perception->SetDominantSense(SightConfig->GetSenseImplementation());
    Perception->OnTargetPerceptionUpdated.AddDynamic(this, &AGuardCharacter::OnTargetPerceptionUpdated);

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AGuardCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Track the first MlCharacter as the player (swap to your own player-finding if needed)
    if (ACharacter* P = UGameplayStatics::GetPlayerCharacter(this, 0))
    {
        TrackedPlayer = Cast<AMlCharacter>(P);
    }
}

void AGuardCharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // We don’t rely solely on this; Tick() also checks extra LOS.
    // But caching a player here is handy in case BeginPlay didn’t find it yet.
    if (!TrackedPlayer.IsValid())
    {
        if (AMlCharacter* Maybe = Cast<AMlCharacter>(Actor))
        {
            TrackedPlayer = Maybe;
        }
    }
}

bool AGuardCharacter::HasClearLineOfSightToPlayer(const AMlCharacter* Player) const
{
    if (!Player) return false;

    FVector EyesLoc; FRotator EyesRot;
    GetActorEyesViewPoint(EyesLoc, EyesRot);

    float HalfHeight = 88.f;
    if (const UCapsuleComponent* PCaps = Player->GetCapsuleComponent())
    {
        HalfHeight = PCaps->GetScaledCapsuleHalfHeight();
    }
    const FVector TargetLoc = Player->GetActorLocation() + FVector(0.0f, 0.0f, HalfHeight * 0.6f);

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(GuardLOS), true);
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(Player);

    const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, EyesLoc, TargetLoc, ECC_Visibility, Params);
    if (!bHit) return true;              // clear path
    return Hit.GetActor() == Player;     // if something was hit, make sure it’s actually the player
}


bool AGuardCharacter::ShouldDetectPlayer(const AMlCharacter* Player) const
{
    if (!Player) return false;

    // Distance gate (optional)
    const float DistSq = FVector::DistSquared(Player->GetActorLocation(), GetActorLocation());
    if (DistSq > MaxDetectRange * MaxDetectRange) return false;

    // AIPerception has its own LOS; we also add a manual trace if desired
    bool bPerceived = false;
    if (Perception)
    {
        TArray<AActor*> Sensed;
        Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), Sensed);
        bPerceived = Sensed.Contains(const_cast<AMlCharacter*>(Player));
    }

    if (!bPerceived)
        return false;

    if (bExtraLOS && !HasClearLineOfSightToPlayer(Player))
        return false;

    // Optional crouch stealth: slower fill if crouching
    return true;
}

void AGuardCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AMlCharacter* Player = TrackedPlayer.Get();
    const bool bExpose = ShouldDetectPlayer(Player);

    float Prev = Detection01;

    if (bExpose)
    {
        float Rate = 1.f / FMath::Max(KINDA_SMALL_NUMBER, FillTimeSeconds);

        // If player crouched, make it slower to fill
        if (Player && Player->bIsCrouched)
        {
            Rate *= CrouchFillMultiplier; // e.g., 0.5 => needs ~10s while crouched
        }

        Detection01 = FMath::Clamp(Detection01 + Rate * DeltaSeconds, 0.f, 1.f);
    }
    else
    {
        Detection01 = FMath::Clamp(Detection01 - DecayPerSecond * DeltaSeconds, 0.f, 1.f);
    }

    if (!FMath::IsNearlyEqual(Detection01, Prev))
    {
        OnDetectionChanged.Broadcast(Detection01);
    }

    // Edge signals
    const bool bIsFull = FMath::IsNearlyEqual(Detection01, 1.f);
    const bool bIsZero = FMath::IsNearlyZero(Detection01);
    if (bIsFull && !bWasFullLastTick)
    {
        OnDetectedPlayer.Broadcast();
    }
    if (bIsZero && !bWasZeroLastTick)
    {
        OnLostPlayer.Broadcast();
    }
    bWasFullLastTick = bIsFull;
    bWasZeroLastTick = bIsZero;
}
