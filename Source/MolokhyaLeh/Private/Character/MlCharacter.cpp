// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MlCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/InputHandelComponent.h"
#include "Character/Components/InteractionComponent.h"
#include "Character/Components/PlayerMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/Inventory.h"

AMlCharacter::AMlCharacter()
{
	PrimaryActorTick.bCanEverTick = true;


	this->CameraArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	this->CameraArmComponent->SetupAttachment(this->RootComponent.Get());


	this->CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	this->CameraComponent->SetupAttachment(this->CameraArmComponent.Get(), USpringArmComponent::SocketName);

	this->InputHandelComponent = CreateDefaultSubobject<UInputHandelComponent>(TEXT("InputHandel"));
	this->PlayerMovementComponent = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("PlayerMovement"));
	this->InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
	
}

void AMlCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMlCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InputHandelComponent)
		{
			InputHandelComponent->SetupInputs(EnhancedInputComponent, this);
		}
	}

}

