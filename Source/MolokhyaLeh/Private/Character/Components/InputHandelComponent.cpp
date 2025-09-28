// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/InputHandelComponent.h"

#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "Character/MlCharacter.h"
#include "Character/Components/InteractionComponent.h"
#include "Character/Components/PlayerMovementComponent.h"

// Sets default values for this component's properties
UInputHandelComponent::UInputHandelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}



void UInputHandelComponent::BeginPlay()
{
	Super::BeginPlay();
	this->PlayerController = Cast<APlayerController>(this->GetOuterAMlCharacter()->GetController());

	if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			InputLocalPlayerSubsystem->AddMappingContext(this->inputMappingContext.Get(), 0);
		}
	}
}



void UInputHandelComponent::SetupInputs(UEnhancedInputComponent* EnhancedInputComponent, AMlCharacter* Character)
{
	if (Character->PlayerMovementComponent)
	{
		EnhancedInputComponent->BindAction(IA_Move.Get(),   ETriggerEvent::Triggered, Character->PlayerMovementComponent.Get(), &UPlayerMovementComponent::Move);
		EnhancedInputComponent->BindAction(IA_Look.Get(),   ETriggerEvent::Triggered, Character->PlayerMovementComponent.Get(), &UPlayerMovementComponent::Look);
		EnhancedInputComponent->BindAction(IA_Jump.Get(),   ETriggerEvent::Started,   Character->PlayerMovementComponent.Get(), &UPlayerMovementComponent::JumpStart);
		EnhancedInputComponent->BindAction(IA_Jump.Get(),   ETriggerEvent::Completed, Character->PlayerMovementComponent.Get(), &UPlayerMovementComponent::JumpStop);
		EnhancedInputComponent->BindAction(IA_Run.Get(),    ETriggerEvent::Started,   Character->PlayerMovementComponent.Get(), &UPlayerMovementComponent::RunStart);
		EnhancedInputComponent->BindAction(IA_Run.Get(),    ETriggerEvent::Completed, Character->PlayerMovementComponent.Get(), &UPlayerMovementComponent::RunStop);
		EnhancedInputComponent->BindAction(IA_Crouch.Get(), ETriggerEvent::Started,   Character->PlayerMovementComponent.Get(), &UPlayerMovementComponent::CrouchStart);
		EnhancedInputComponent->BindAction(IA_Crouch.Get(), ETriggerEvent::Completed, Character->PlayerMovementComponent.Get(), &UPlayerMovementComponent::CrouchStop);
	}


	if (Character->InteractionComponent)
	{
		EnhancedInputComponent->BindAction(IA_Interact.Get(), ETriggerEvent::Started, Character->InteractionComponent.Get(), &UInteractionComponent::Interact);
	}

}

