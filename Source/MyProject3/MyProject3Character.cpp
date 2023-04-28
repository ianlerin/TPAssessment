// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject3Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "MyProject3/Component/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "MyProject3/Widget/SpeakWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AMyProject3Character

AMyProject3Character::AMyProject3Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	InteractWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("InteractWidgetComponent"));
	InteractWidgetComponent->SetupAttachment(RootComponent);
	InteractWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	PickupAttachPoint = CreateDefaultSubobject<USceneComponent>(FName("PickupAttachPoint"));
	PickupAttachPoint->SetupAttachment(RootComponent);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMyProject3Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	GetWorld()->SpawnActor<AActor>();
}


void AMyProject3Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	USpeakWidget* SpeakWidget = Cast<USpeakWidget>(InteractWidgetComponent->GetUserWidgetObject());
	if (SpeakWidget)
	{
		if (SpeakWidget->IsActive())
		{
			// adjust interact widget to always face the player if widget is active
			auto PlayerManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
			FRotator FoundRot = UKismetMathLibrary::FindLookAtRotation(InteractWidgetComponent->GetComponentLocation(), PlayerManager->GetCameraLocation());
			InteractWidgetComponent->SetWorldRotation(FRotator(0, FoundRot.Yaw, 0));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProject3Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyProject3Character::CustomJump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(JetpackAction, ETriggerEvent::Started, this, &AMyProject3Character::ActivateJetPack);
		EnhancedInputComponent->BindAction(JetpackAction, ETriggerEvent::Completed, this, &AMyProject3Character::StopJetPack);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMyProject3Character::TryInteract);
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &AMyProject3Character::PlayPunch);
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProject3Character::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProject3Character::Look);

	}

}

void AMyProject3Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyProject3Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyProject3Character::ActivateJetPack()
{
	FTimerDelegate JetpackDelegate;
	JetpackDelegate.BindUFunction(this, "JetpackFly");
	GetWorldTimerManager().SetTimer(JetpackTimerHandler, JetpackDelegate, JetpackThrustInterval, true);
}

void AMyProject3Character::JetpackFly()
{
	FVector ToUse = FVector(0, 0, 1) * JetpackStrength;
	LaunchCharacter(ToUse, false, true);
}

void AMyProject3Character::StopJetPack()
{
	GetWorldTimerManager().ClearTimer(JetpackTimerHandler);
}

void AMyProject3Character::CustomJump()
{
	if (JumpCount >= MaxJumpCount || GetWorldTimerManager().IsTimerActive(JetpackTimerHandler))
	{
		return;
	}
	JumpCount++;
	FVector ToUse = FVector(0, 0, 1);
	if (JumpCount == 1)
	{
		ToUse = ToUse * FirstJumpStr;
	}
	else
	{
		ToUse = ToUse * SecondJumpStr;
	}
	LaunchCharacter(ToUse, false, true);
}

void AMyProject3Character::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		JumpCount = 0;
	}
}


void AMyProject3Character::TryInteract()
{
	// if has item picked up, run the interact function again and  throw the pickup item
	if (InteractComponent->PickedUpActor)
	{
		UInteractComponent* PickupInteractComp = Cast<UInteractComponent>(InteractComponent->PickedUpActor->FindComponentByClass<UInteractComponent>());
		if (PickupInteractComp)
		{
			PickupInteractComp->Interact(this, FollowCamera->GetForwardVector());
		}
	}
	// if no item pickup, then try to trace forward to find object that can be interacted to interact
	else
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		FHitResult Res;
		FVector TraceStart = GetActorLocation();
		FVector TraceEnd = GetActorLocation() + FollowCamera->GetForwardVector() * InteractComponent->InteractRange;
		UKismetSystemLibrary::CapsuleTraceSingle(this, TraceStart, TraceEnd, 20,20,ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Res, false);
		if (Res.GetActor())
		{
			UInteractComponent* InteractComp = Cast<UInteractComponent>(Res.GetActor()->FindComponentByClass<UInteractComponent>());
			if (InteractComp)
			{
				InteractComp->Interact(this, FollowCamera->GetForwardVector());
			}
		}
	}
	
}


void AMyProject3Character::OnInteract()
{
	USpeakWidget* SpeakWidget = Cast<USpeakWidget>(InteractWidgetComponent->GetUserWidgetObject());
	if (SpeakWidget)
	{
		SpeakWidget->SetActive(true);
	}
}

void AMyProject3Character::OnEndInteract()
{
	USpeakWidget* SpeakWidget = Cast<USpeakWidget>(InteractWidgetComponent->GetUserWidgetObject());
	if (SpeakWidget)
	{
		SpeakWidget->SetActive(false);
	}
}

FVector AMyProject3Character::GetThrowForwardVector()
{
	return FollowCamera->GetForwardVector();
}

void AMyProject3Character::TryPickup(AActor* ToPickup)
{
	if (ToPickup)
	{
		FAttachmentTransformRules AttachRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,false);
		ToPickup->AttachToComponent(PickupAttachPoint, AttachRules);
	}

}

void AMyProject3Character::PlayPunch()
{
	if (GetWorldTimerManager().IsTimerActive(MeleePunchHandler))
	{
		return;
	}
	float Len=GetMesh()->GetAnimInstance()->Montage_Play(PunchMontage);

	GetWorldTimerManager().SetTimer(MeleePunchHandler, Len, false);
}


void AMyProject3Character::ExecutePunch()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult Res;
	// slight forward from character
	FVector Start = GetActorLocation() + GetActorForwardVector() * MeleeRange;
	FVector End = Start;
	bool bFound=UKismetSystemLibrary::SphereTraceSingle(this, Start, End, MeleeRadius, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, Res, false);
	if (bFound)
	{
		if (Res.GetActor())
		{
			UHealthComponent* HealthComp = Res.GetActor()->FindComponentByClass<UHealthComponent>();
			if (HealthComp)
			{
				HealthComp->DecreaseHealth(PunchDamage);
			}
		}
	}

}