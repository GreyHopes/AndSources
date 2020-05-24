// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapSensitiveActor.h"
#include "AndPlayerController.h"
#include <Engine/World.h>

// Sets default values
ASwapSensitiveActor::ASwapSensitiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASwapSensitiveActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	AAndPlayerController* playerController = world->GetFirstPlayerController<AAndPlayerController>();
	if (!ensure(playerController != nullptr)) return;

	playerController->OnSwapEvent().AddUObject<ASwapSensitiveActor>(this, &ASwapSensitiveActor::OnSwapEvent);
}

// Called every frame
void ASwapSensitiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

