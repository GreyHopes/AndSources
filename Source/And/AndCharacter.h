// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <Runtime/Engine/Public/CollisionQueryParams.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AndCharacter.generated.h"

#define INTERACT_DISTANCE 200.0f 

class UInputComponent;

UCLASS(config = Game)
class AND_API AAndCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAndCharacter();

	void Interact();

	void HandleHighlight();
	AActor* FindActorInLOS();

private:
	AActor* FocusedActor;
	float InteractionDistance = INTERACT_DISTANCE;
	FCollisionQueryParams TraceParams;


protected:
	virtual void BeginPlay();
	virtual void Tick(float deltaTime);

public:
	/** Whether or not the character should fly */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool IsFlying;

	/** FVector pointing to the other entity (ghost/body) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
		FVector ToOther;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:

	/** Handles moving forward/backward */
	//void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	//void MoveRight(float Val);

	/** Handles elevation movement, up and down */
	//void MoveUp(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	 //void TurnAtRate(float Rate);

	 /**
	  * Called via input to turn look up/down at a given rate.
	  * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	  */
	  //void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

};