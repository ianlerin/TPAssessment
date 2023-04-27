// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject3/Widget/MyHUD.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	UUserWidget* MainHud = CreateWidget<UUserWidget>(PlayerOwner, MainWidget);
	if (MainHud)
	{
		MainHud->AddToViewport();
	}
}