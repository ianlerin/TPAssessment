// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"


// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetOwner()&& InteractedActor)
	{
		// end interaction if too far away
		float Dist = FVector::Distance(GetOwner()->GetActorLocation(), InteractedActor->GetActorLocation());
		if (Dist > MaxInteractRange)
		{
			IInteractInterface* Interface = Cast<IInteractInterface>(GetOwner());
			if (Interface)
			{
				Interface->OnEndInteract();
				bIsInteracting = false;
			}
		}
	}

	// ...
}

void UInteractComponent::Interact(AActor* InteractActor, FVector ForwardVec)
{
	if (bIsInteracting)
	{
		bIsInteracting = false;
		IInteractInterface* Interface = Cast<IInteractInterface>(GetOwner());
		if (Interface)
		{
			Interface->OnEndInteract();
		}
		InteractedActor = nullptr;
	}
	else
	{
		InteractedActor = InteractActor;
		bIsInteracting = true;
		IInteractInterface* Interface = Cast<IInteractInterface>(GetOwner());
		if (Interface)
		{
			Interface->OnInteract();
		}
		
	}
}

