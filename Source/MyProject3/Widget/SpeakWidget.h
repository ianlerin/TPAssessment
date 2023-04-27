// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpeakWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT3_API USpeakWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	bool bIsActive = false;
public:
	void SetActive(bool bToSet);
	bool IsActive()
	{
		return bIsActive;
	}
};
