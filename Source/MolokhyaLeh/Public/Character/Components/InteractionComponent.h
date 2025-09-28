// InteractionComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class AMlCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Within=MlCharacter)
class MOLOKHYALEH_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float TraceDistance = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float SphereRadius = 30.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_GameTraceChannel1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	TObjectPtr<AActor> FocusedActor = nullptr;
	
	void TraceForInteractable();
	
	void Interact();
};
