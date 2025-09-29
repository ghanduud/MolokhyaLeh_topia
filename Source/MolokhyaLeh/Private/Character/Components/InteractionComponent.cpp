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

	this->TraceForInteractable();
}

void UInteractionComponent::TraceForInteractable()
{
	AMlCharacter* Owner = this->GetOuterAMlCharacter();
	if (!Owner) return;

	UCameraComponent* Camera = Owner->CameraComponent;
	if (!Camera) return;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * TraceDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);
	
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	bool bHit = GetWorld()->SweepSingleByObjectType(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ObjectParams,
		FCollisionShape::MakeSphere(SphereRadius),
		Params
	);

	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			FocusedActor = HitActor;
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, SphereRadius, 12, FColor::Green);
		}
	}
	else
	{
		FocusedActor = nullptr;
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow);
}


void UInteractionComponent::Interact()
{
	if (FocusedActor && FocusedActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable::Execute_Interact(FocusedActor, this->GetOuterAMlCharacter());
	}
}
