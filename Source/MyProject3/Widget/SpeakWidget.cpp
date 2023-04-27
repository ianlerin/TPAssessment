// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject3/Widget/SpeakWidget.h"

void USpeakWidget::SetActive(bool bToSet)
{
	bIsActive = bToSet;
	if (bIsActive)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}