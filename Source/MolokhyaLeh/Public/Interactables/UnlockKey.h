#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Inventory/Inventory.h"
#include "UnlockKey.generated.h"


UCLASS()
class MOLOKHYALEH_API AUnlockKey : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AUnlockKey();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AMlCharacter* Interactor) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	FName KeyId = "Key_Door01";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	FText KeyDisplayName = FText::FromString("Door Key");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	EItemType ItemType = EItemType::Key;

};
