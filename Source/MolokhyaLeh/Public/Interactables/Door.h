// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void Interact_Implementation(AMlCharacter* Interactor) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock")
	bool bLocked = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock")
	FName RequiredKeyId = "Key_Door01";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock")
	bool bConsumeKeyOnUse = true;

	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor();
};
