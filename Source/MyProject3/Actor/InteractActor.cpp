// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject3/Actor/InteractActor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
AInteractActor::AInteractActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	ProjMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjMovement"));

}

// Called when the game starts or when spawned
void AInteractActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractActor::OnInteract()
{
	if (InteractComponent->InteractedActor)
	{
		IInteractInterface* InteractActorInterface = Cast<IInteractInterface>(InteractComponent->InteractedActor);
		UInteractComponent* InteractorComp = (InteractComponent->InteractedActor->FindComponentByClass<UInteractComponent>());
		if (!InteractorComp->PickedUpActor)
		{
			if (InteractorComp)
			{
				InteractorComp->PickedUpActor = this;
			}
			// disable collision, deactivate movement and attach to character
			ProjMovementComp->SetUpdatedComponent(StaticMesh);
			ProjMovementComp->Deactivate();
			StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			if (InteractActorInterface)
			{
				InteractActorInterface->TryPickup(this);
			}
		
		}
	}
}

void AInteractActor::OnEndInteract()
{
	if (InteractComponent->InteractedActor)
	{
		if (ProjMovementComp)
		{
			IInteractInterface* Interface = Cast<IInteractInterface>(InteractComponent->InteractedActor);
			if (Interface)
			{
				// detach actor from pickedup component, set velocity, activate movement
				FVector ToSet = Interface->GetThrowForwardVector() * LaunchSpeed;
				ProjMovementComp->Velocity = ToSet;
				ProjMovementComp->SetUpdatedComponent(StaticMesh);
				DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				ProjMovementComp->Activate();
				StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
			UInteractComponent* InteractorComp =(InteractComponent->InteractedActor->FindComponentByClass<UInteractComponent>());
			if (InteractorComp)
			{
				InteractorComp->PickedUpActor = nullptr;
			}
		
		}
	}
}

FVector AInteractActor::GetThrowForwardVector()
{
	return GetActorForwardVector();
}