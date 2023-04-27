// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyProject3/Interface/InteractInterface.h"
#include "InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT3_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();
	UPROPERTY(EditDefaultsOnly)
	FName HeadSocket = "headSocket";
	UPROPERTY(EditDefaultsOnly)
	float InteractRange = 1000;
	UPROPERTY(EditDefaultsOnly)
	float MaxInteractRange = 2500;
	bool bIsInteracting = false;
	// actor that initiated the interact action
	UPROPERTY()
	AActor* InteractedActor = nullptr;
	// picked up actor
	UPROPERTY()
	AActor* PickedUpActor = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly)
	bool bCanPickup = false;

	UPROPERTY(EditDefaultsOnly)
	FVector AttachForwardOffset = FVector(100,0,0);
	UPROPERTY(EditDefaultsOnly)
	float LaunchSpeed = 3000;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Interact(AActor* InteractActor, FVector ForwardVec);

	void TryInteract();
};
