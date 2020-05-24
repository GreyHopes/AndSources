#include "InterActor.h"
#include "Engine.h"

AInterActor::AInterActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AInterActor::BeginPlay()
{
	AActor::BeginPlay();

	for (UActorComponent* Comp : GetComponentsByClass(UMeshComponent::StaticClass()))
	{
		UMeshComponent* Mesh = Cast<UMeshComponent>(Comp);
		Meshes.Push(Mesh);
	}

}

void AInterActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

void AInterActor::OnInteract_Implementation(AActor* Caller)
{
	Destroy();
}

void AInterActor::StartFocus_Implementation()
{
	for (UMeshComponent* Mesh : Meshes)
	{
		Mesh->SetRenderCustomDepth(true);
		GEngine->AddOnScreenDebugMessage(0, 5, FColor::Red, FString::Printf(TEXT("Focus On")));
	}
}

void AInterActor::EndFocus_Implementation()
{
	for (UMeshComponent* Mesh : Meshes)
	{
		Mesh->SetRenderCustomDepth(false);
		GEngine->AddOnScreenDebugMessage(0, 5, FColor::Red, FString::Printf(TEXT("Focus Off")));
	}
}