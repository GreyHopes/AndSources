// Fill out your copyright notice in the Description page of Project Settings.


#include "AndPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Engine/World.h>
#include "AndCharacter.h"

AAndPlayerController::AAndPlayerController() : Super()
{
	CurrentCharacterState = CharacterState::PlayerState;
	MaxMoveDistance = 1000.0f;
	SquaredMaxMoveDistance = MaxMoveDistance * MaxMoveDistance;
	SetActorTickEnabled(true);
	CharactersMerged = true;
}


void AAndPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerInstance = Cast<AAndCharacter>(GetCharacter());
	if (!ensure(PlayerInstance != nullptr)) return;

	if (!ensure(GhostBP != nullptr)) return;

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	GhostInstance = world->SpawnActor<AAndCharacter>(GhostBP, PlayerInstance->GetTransform());
	if (!ensure(GhostInstance != nullptr)) return;
}


void AAndPlayerController::Tick(float DeltaSeconds)
{
	FVector playerToGhost = GhostInstance->GetActorLocation() - PlayerInstance->GetActorLocation();
	FVector ghostToPlayer = PlayerInstance->GetActorLocation() - GhostInstance->GetActorLocation();

	// If ghost is too far away, move it within max range
	if (CurrentCharacterState == CharacterState::GhostState && 
		playerToGhost.SizeSquared() > SquaredMaxMoveDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("MOVE %f"), playerToGhost.SizeSquared());
		OnLimitReached();
		GhostInstance->SetActorLocation(PlayerInstance->GetActorLocation() + playerToGhost.GetClampedToMaxSize(MaxMoveDistance), false, nullptr, ETeleportType::ResetPhysics);
	}

	// If player is too far away, move it within max range
	if (CurrentCharacterState == CharacterState::PlayerState &&
		playerToGhost.SizeSquared() > SquaredMaxMoveDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("MOVE %f"), playerToGhost.SizeSquared());
		OnLimitReached();
		PlayerInstance->SetActorLocation(GhostInstance->GetActorLocation() + ghostToPlayer.GetClampedToMaxSize(MaxMoveDistance), false, nullptr, ETeleportType::ResetPhysics);
	}

	// Compute distance again and pass it to characters
	GhostInstance->ToOther = ghostToPlayer.GetSafeNormal(SquaredMaxMoveDistance);
	PlayerInstance->ToOther = playerToGhost.GetSafeNormal(SquaredMaxMoveDistance);
}


float AAndPlayerController::DistanceBetweenCharacters()
{
	return FVector::Dist(PlayerInstance->GetActorLocation(), GhostInstance->GetActorLocation());
}

bool AAndPlayerController::CanGoToNextLevel(float minDistance = 0)
{
	return CharactersMerged || DistanceBetweenCharacters() < minDistance;
}


void AAndPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UInputComponent* PlayerInputComponent = InputComponent;

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("SwapCharacters", IE_Pressed, this, &AAndPlayerController::OnStartSwap);
}


void AAndPlayerController::SwapCharacters()
{
	if (DistanceBetweenCharacters() > MaxMoveDistance)
		return;

	StopMovement();

	if (CurrentCharacterState == CharacterState::PlayerState)
	{
		CurrentCharacterState = CharacterState::GhostState;
		Possess(GhostInstance);
	}
	else if (CurrentCharacterState == CharacterState::GhostState)
	{
		CurrentCharacterState = CharacterState::PlayerState;
		Possess(PlayerInstance);
	}

	SwapEvent.Broadcast(CurrentCharacterState);
}

CharacterState AAndPlayerController::GetCurrentCharacterState()
{
	return CurrentCharacterState;
}

AAndCharacter* AAndPlayerController::GetPlayerCharacter()
{
	return PlayerInstance;
}

AAndCharacter* AAndPlayerController::GetGhostCharacter()
{
	return GhostInstance;
}

float AAndPlayerController::GetMaxMoveDistance()
{
	return MaxMoveDistance;
}

#if WITH_EDITOR  
void AAndPlayerController::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	//Get the name of the property that was changed  
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// We test using GET_MEMBER_NAME_CHECKED so that if someone changes the property name  
	// in the future this will fail to compile and we can update it.  
	if ((PropertyName == GET_MEMBER_NAME_CHECKED(AAndPlayerController, MaxMoveDistance)))
	{
		SquaredMaxMoveDistance = MaxMoveDistance * MaxMoveDistance;
	}

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif