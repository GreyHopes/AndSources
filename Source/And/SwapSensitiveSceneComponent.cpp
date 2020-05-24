// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapSensitiveSceneComponent.h"

#include "AndPlayerController.h"

// Sets default values for this component's properties
USwapSensitiveSceneComponent::USwapSensitiveSceneComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USwapSensitiveSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	AAndPlayerController* playerController = world->GetFirstPlayerController<AAndPlayerController>();
	if (!ensure(playerController != nullptr)) return;

	playerController->OnSwapEvent().AddUObject<USwapSensitiveSceneComponent>(this, &USwapSensitiveSceneComponent::OnSwapEvent);
}


// Called every frame
void USwapSensitiveSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

