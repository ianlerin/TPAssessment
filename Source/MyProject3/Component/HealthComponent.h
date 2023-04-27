// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT3_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	FOnHealthChanged OnHealthChangedDelegate;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DecreaseHealth(float ToDecrease);

	UPROPERTY(EditDefaultsOnly)
	float Health = 100;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
};
