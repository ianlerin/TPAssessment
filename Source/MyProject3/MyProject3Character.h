// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MyProject3/Component/InteractComponent.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyProject3/Interface/InteractInterface.h"
#include "MyProject3Character.generated.h"


UCLASS(config=Game)
class AMyProject3Character : public ACharacter,public IInteractInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JetpackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MeleeAction;
	
public:
	AMyProject3Character();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interact")
	UInteractComponent* InteractComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interact")
	class UWidgetComponent* InteractWidgetComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interact")
	USceneComponent* PickupAttachPoint;
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	//jetpack
	UPROPERTY()
	FTimerHandle JetpackTimerHandler;
	UPROPERTY()
	FTimerHandle MeleePunchHandler;
	UPROPERTY(EditDefaultsOnly,category = "Jetpack")
	float JetpackThrustInterval = 0.1;
	UPROPERTY(EditDefaultsOnly, category = "Jetpack")
	float JetpackStrength = 100;
	UFUNCTION()
	void JetpackFly();
	void StopJetPack();
	void ActivateJetPack();
	//jump
	UPROPERTY(EditDefaultsOnly, category = "Jump")
	int MaxJumpCount = 1;
	UPROPERTY(EditDefaultsOnly, category = "Jump")
	float FirstJumpStr = 700;
	UPROPERTY(EditDefaultsOnly, category = "Jump")
	float SecondJumpStr = 700;
	int JumpCount = 0;

	void CustomJump();
	// interact
	void TryInteract();
	virtual void OnInteract() override;
	virtual void OnEndInteract() override;
	virtual FVector GetThrowForwardVector() override;
	virtual void TryPickup(AActor* ToPickup) override;
	//punch
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* PunchMontage = nullptr;
	void PlayPunch();
	// called in anim notify bp
	UFUNCTION(BlueprintCallable)
	void ExecutePunch();

	UPROPERTY(EditDefaultsOnly,category="Melee")
	float MeleeRadius = 100;

	UPROPERTY(EditDefaultsOnly, category = "Melee")
	float MeleeRange = 100;

	UPROPERTY(EditDefaultsOnly, category = "Melee")
	float PunchDamage = 50;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

