// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProject3/Component/InteractComponent.h"
#include "MyProject3/Interface/InteractInterface.h"
#include "InteractActor.generated.h"

UCLASS()
class MYPROJECT3_API AInteractActor : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractActor();
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interact")
	UInteractComponent* InteractComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProjectileMovementComponent* ProjMovementComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditDefaultsOnly)
	FName HeadSocket = "headSocket";
	UPROPERTY(EditDefaultsOnly)
	float LaunchSpeed = 1000;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// interact
	virtual void OnInteract() override;
	virtual void OnEndInteract() override;
	virtual FVector GetThrowForwardVector() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
