// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/PlayerMovementComponent.h"

#include "InputActionValue.h"
#include "Character/MlCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerMovementComponent::UPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	this->MPlayerCharacter = this->GetOuterAMlCharacter();
	this->MoveComp = this->MPlayerCharacter->GetCharacterMovement();

	this->MoveComp->MaxWalkSpeed = this->WalkSpeed;
	this->MoveComp->RotationRate = this->RotationSpeed;
	this->MoveComp->MaxWalkSpeedCrouched = this->CrouchedSpeed;
}


void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->MoveComp->IsFalling())
	{
		this->MoveComp->RotationRate = this->RotationSpeedInAir;
	}
	else
	{
		this->MoveComp->RotationRate = this->RotationSpeed;
	}
}

void UPlayerMovementComponent::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	AController* MyController = this->MPlayerCharacter->GetController();
	if (!MyController) return;

	const FRotator Rotation = MyController->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


	this->MPlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	this->MPlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void UPlayerMovementComponent::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	LookVector *= MouseSensitivity;

	MPlayerCharacter->AddControllerYawInput(LookVector.X);
	MPlayerCharacter->AddControllerPitchInput(LookVector.Y);
}

void UPlayerMovementComponent::JumpStart()
{
	this->MPlayerCharacter->Jump();
}


void UPlayerMovementComponent::JumpStop()
{
	this->MPlayerCharacter->StopJumping();
}

void UPlayerMovementComponent::RunStart()
{
	MoveComp->MaxWalkSpeed = this->RunSpeed;
}


void UPlayerMovementComponent::RunStop()
{
	MoveComp->MaxWalkSpeed = this->WalkSpeed;
}

void UPlayerMovementComponent::CrouchStart()
{
	if (this->MPlayerCharacter->CanCrouch())
	{
		this->MPlayerCharacter->Crouch();
	}
}


void UPlayerMovementComponent::CrouchStop()
{
	this->MPlayerCharacter->UnCrouch();
}
