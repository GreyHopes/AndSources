// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AndCharacter.h"
#include "AndProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "InteractInterface.h" 

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AAndCharacter

AAndCharacter::AAndCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	/* Initialize attributes for interaction*/
	TraceParams = FCollisionQueryParams(FName(TEXT("Trace")), true, this);

	PrimaryActorTick.bCanEverTick = true;

}

void AAndCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AAndCharacter::Tick(float deltaTime) {
	Super::Tick(deltaTime);
	if (IsLocallyControlled())
		HandleHighlight();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAndCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up gameplay key bindings
	check(PlayerInputComponent);
	

	// Bind jump events
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	//PlayerInputComponent->BindAxis("MoveForward", this, &AAndCharacter::MoveForward);
	//PlayerInputComponent->BindAxis("MoveRight", this, &AAndCharacter::MoveRight);

	//if (IsFlying)
	//{
	//PlayerInputComponent->BindAxis("MoveUp", this, &AAndCharacter::MoveUp);
	//}

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AAndCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AAndCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AAndCharacter::Interact);
}

//void AAndCharacter::MoveForward(float Value)
//{
//	if (Value != 0.0f)
//	{
//		float scale = FMath::Abs(Value);
//		FVector dir = FMath::Sign(Value) * GetActorForwardVector();
//
//		if (!ToOther.IsZero())
//		{
//			scale = FMath::Clamp(FVector::DotProduct(dir, ToOther), 0.f, 1.f);
//		}
//
//		AddMovementInput(dir, scale);
//	}
//}

//void AAndCharacter::MoveRight(float Value)
//{
//	if (Value != 0.0f)
//	{
//		float scale = FMath::Abs(Value);
//		FVector dir = FMath::Sign(Value) * GetActorRightVector();
//
//		if (!ToOther.IsZero())
//		{
//			scale = FMath::Clamp(FVector::DotProduct(dir, ToOther), 0.f, 1.f);
//		}
//
//		AddMovementInput(dir, scale);
//	}
//}

//void AAndCharacter::MoveUp(float Value)
//{
//	if (Value != 0.0f)
//	{
//		float scale = FMath::Abs(Value);
//		FVector dir = FMath::Sign(Value) * GetActorUpVector();
//
//		if (!ToOther.IsZero())
//		{
//			scale = FMath::Clamp(FVector::DotProduct(dir, ToOther), 0.f, 1.f);
//		}
//
//		AddMovementInput(dir, scale);
//	}
//}

//void AAndCharacter::TurnAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
//}

//void AAndCharacter::LookUpAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
//}

void AAndCharacter::Interact() {
	if (FocusedActor) {
		IInteractInterface* interface = Cast<IInteractInterface>(FocusedActor);
		if (interface) {
			interface->Execute_OnInteract(FocusedActor, this);
		}
	}
}

AActor* AAndCharacter::FindActorInLOS() {
	if (!Controller) {
		return nullptr;
	}

	FVector Loc;
	FRotator Rot;
	FHitResult Hit(ForceInit);
	GetController()->GetPlayerViewPoint(Loc, Rot);

	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * INTERACT_DISTANCE);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	return Hit.GetActor();
}

void AAndCharacter::HandleHighlight()
{
	AActor* interactable = FindActorInLOS();

	if (interactable)
	{
		if (interactable != FocusedActor)
		{
			if (FocusedActor)
			{
				IInteractInterface* interface = Cast<IInteractInterface>(FocusedActor);
				if (interface)
				{
					interface->Execute_EndFocus(FocusedActor);
				}
			}
			IInteractInterface* interface = Cast<IInteractInterface>(interactable);
			if (interface)
			{
				interface->Execute_StartFocus(interactable);
			}
			FocusedActor = interactable;
		}
	}
	else
	{
		if (FocusedActor)
		{
			IInteractInterface* interface = Cast<IInteractInterface>(FocusedActor);
			if (interface)
			{
				interface->Execute_EndFocus(FocusedActor);
			}
		}
		FocusedActor = nullptr;
	}
}