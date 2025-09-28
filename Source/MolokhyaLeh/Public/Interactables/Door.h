// Door.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Door.generated.h"

class AMlCharacter;

UCLASS()
class MOLOKHYALEH_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ADoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Interaction
	virtual void Interact_Implementation(AMlCharacter* Interactor) override;

	// Flags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsLocked = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	bool bIsOpen = false;

	// Door functions
	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor();

	UFUNCTION(BlueprintCallable, Category = "Door")
	void CloseDoor();
};
