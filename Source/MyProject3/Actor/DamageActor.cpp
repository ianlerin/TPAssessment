// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject3/Actor/DamageActor.h"
#include "MyProject3/Component/HealthComponent.h"
// Sets default values
ADamageActor::ADamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
    HealthComp= CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
}

// Called when the game starts or when spawned
void ADamageActor::BeginPlay()
{
	Super::BeginPlay();
	HealthComp->OnHealthChangedDelegate.AddDynamic(this, &ADamageActor::OnHealthDecrease);
	MaterialInst=StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
}

// Called every frame
void ADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageActor::OnHealthDecrease(float NewHealth)
{
	if (NewHealth <= 0)
	{
		Destroy();
	}
	else
	{
		float Percent = NewHealth / HealthComp->MaxHealth;
		if (MaterialInst)
		{
			MaterialInst->SetScalarParameterValue("HealthPercent", Percent);
		}
	}
}