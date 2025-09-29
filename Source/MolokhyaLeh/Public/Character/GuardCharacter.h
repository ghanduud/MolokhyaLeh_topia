#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GuardCharacter.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class AMlCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuardDetectionChanged, float, Detection01);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGuardDetectedPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGuardLostPlayer);

UCLASS()
class MOLOKHYALEH_API AGuardCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AGuardCharacter();

    virtual void Tick(float DeltaSeconds) override;

    // Fired every time the normalized detection value changes (0..1)
    UPROPERTY(BlueprintAssignable, Category = "Guard|Detection")
    FOnGuardDetectionChanged OnDetectionChanged;

    // Fired once when Detection01 reaches 1
    UPROPERTY(BlueprintAssignable, Category = "Guard|Detection")
    FOnGuardDetectedPlayer OnDetectedPlayer;

    // Fired once when going from >0 to 0
    UPROPERTY(BlueprintAssignable, Category = "Guard|Detection")
    FOnGuardLostPlayer OnLostPlayer;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    // Strong LOS check that respects walls/props
    bool HasClearLineOfSightToPlayer(const AMlCharacter* Player) const;

    // True if the guard should be “building detection” this tick
    bool ShouldDetectPlayer(const AMlCharacter* Player) const;

private:
    // --- Perception ---
    UPROPERTY(VisibleAnywhere, Category = "Guard|Perception")
    UAIPerceptionComponent* Perception = nullptr;

    UPROPERTY()
    UAISenseConfig_Sight* SightConfig = nullptr;

    // --- Detection model ---
    // Time to fully detect from 0 -> 1 while exposed
    UPROPERTY(EditAnywhere, Category = "Guard|Detection")
    float FillTimeSeconds = 5.f;

    // How fast detection drains per second when not exposed
    UPROPERTY(EditAnywhere, Category = "Guard|Detection")
    float DecayPerSecond = 0.5f;

    // Extra requirement: player must be within this range to start filling (optional)
    UPROPERTY(EditAnywhere, Category = "Guard|Detection")
    float MaxDetectRange = 2500.f;

    // If true, we add our own Visibility trace on top of AIPerception LOS
    UPROPERTY(EditAnywhere, Category = "Guard|Detection")
    bool bExtraLOS = true;

    // If player is crouched, slow the fill (optional stealth bonus)
    UPROPERTY(EditAnywhere, Category = "Guard|Detection")
    float CrouchFillMultiplier = 0.5f;

    // Runtime state
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Guard|Detection", meta = (AllowPrivateAccess = "true"))
    float Detection01 = 0.f; // 0..1

    UPROPERTY()
    TWeakObjectPtr<AMlCharacter> TrackedPlayer;

    bool bWasFullLastTick = false;
    bool bWasZeroLastTick = true;
};
