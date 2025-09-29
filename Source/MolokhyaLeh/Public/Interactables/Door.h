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

	// Allow Blueprint override but provide C++ default
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Door")
	void OpenDoor();
	virtual void OpenDoor_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Door")
	void CloseDoor();
	virtual void CloseDoor_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock")
	FName RequiredKeyId = "Key_Door01";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock")
	bool bConsumeKeyOnUse = true;
};
