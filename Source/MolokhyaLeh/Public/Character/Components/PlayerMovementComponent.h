// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMovementComponent.generated.h"


class AMlCharacter;
class UCharacterMovementComponent;
struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Within=MlCharacter )
class MOLOKHYALEH_API UPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerMovementComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentControl", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentControl", meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 800.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentControl", meta = (AllowPrivateAccess = "true"))
	float CrouchedSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentControl", meta = (AllowPrivateAccess = "true"))
	FRotator RotationSpeed = FRotator(0.f, 540.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentControl", meta = (AllowPrivateAccess = "true"))
	FRotator RotationSpeedInAir = FRotator(0.f, 120.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentControl", meta = (AllowPrivateAccess = "true"))
	float MouseSensitivity = 0.5f;
	
	
	
	
protected:
	
	UPROPERTY()
	TObjectPtr<AMlCharacter> MPlayerCharacter = nullptr;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MoveComp = nullptr;

public:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
	void JumpStart();
	
	UFUNCTION()
	void JumpStop();
	
	UFUNCTION()
	void RunStart();
	
	UFUNCTION()
	void RunStop();
	
	UFUNCTION()
	void CrouchStart();
	
	UFUNCTION()
	void CrouchStop();
};
