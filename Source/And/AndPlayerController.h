// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AndCharacter.h"
#include "And.h"
#include "AndPlayerController.generated.h"

/**
 *
 */

 /* Represent in which state the character currently is (controling his physical or spiritual body) */

UCLASS()
class AND_API AAndPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAndPlayerController();

protected:
	void BeginPlay() override;

	void SetupInputComponent() override;

	void Tick(float DeltaSeconds) override;

private:

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DistanceBetweenCharacters"))
	float DistanceBetweenCharacters();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CanGoToNextLevel"))
	bool CanGoToNextLevel(float minDistance);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SwapCharacters"))
	void SwapCharacters();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStartSwap"))
	void OnStartSwap();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnLimitReached"))
	void OnLimitReached();

	DECLARE_EVENT_OneParam(AAndPlayerController, FSwapEvent, CharacterState)
	FSwapEvent& OnSwapEvent() { return SwapEvent; }

	UFUNCTION(BlueprintGetter)
	CharacterState GetCurrentCharacterState();
	
	UFUNCTION(BlueprintGetter)
	AAndCharacter* GetPlayerCharacter();
	
	UFUNCTION(BlueprintGetter)
	AAndCharacter* GetGhostCharacter();

	UFUNCTION(BlueprintGetter)
	float GetMaxMoveDistance();

	/** Ghost blueprint */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Ghost)
	bool CharactersMerged;

private:
	/** Ghost blueprint */
	UPROPERTY(EditAnywhere, Category = Ghost)
	TSubclassOf<AAndCharacter> GhostBP;

	UPROPERTY(BlueprintGetter = GetPlayerCharacter, Category = Ghost)
	AAndCharacter* PlayerInstance;

	UPROPERTY(BlueprintGetter = GetGhostCharacter, Category = Ghost)
	AAndCharacter* GhostInstance;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxMoveDistance, Category = Ghost)
	float MaxMoveDistance;
	float SquaredMaxMoveDistance;

	UPROPERTY(BlueprintGetter = GetCurrentCharacterState, Category = Ghost)
	CharacterState CurrentCharacterState;

	FSwapEvent SwapEvent;

#if WITH_EDITOR
protected:
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
#endif
};
