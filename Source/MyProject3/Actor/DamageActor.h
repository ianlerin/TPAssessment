// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageActor.generated.h"

UCLASS()
class MYPROJECT3_API ADamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageActor();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UHealthComponent* HealthComp;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInst = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHealthDecrease(float NewHealth);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
