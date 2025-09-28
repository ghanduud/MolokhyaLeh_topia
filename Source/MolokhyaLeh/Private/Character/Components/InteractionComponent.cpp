#include "Character/Components/InteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Character/MlCharacter.h"
#include "Interfaces/Interactable.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceForInteractable();
}

void UInteractionComponent::TraceForInteractable()
{
	AMlCharacter* Owner = Cast<AMlCharacter>(GetOwner());
	if (!Owner) return;

	UCameraComponent* Camera = Owner->CameraComponent;
	if (!Camera) return;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * TraceDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(SphereRadius),
		Params
	);

	if (bHit)
	{
		FocusedActor = Hit.GetActor();
		// Debug
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, SphereRadius, 12, FColor::Green);
	}
	else
	{
		FocusedActor = nullptr;
	}

	// Debug line
	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow);
}

void UInteractionComponent::Interact()
{
	if (FocusedActor && FocusedActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable::Execute_Interact(FocusedActor, GetOwner());
	}
}
