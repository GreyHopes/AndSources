#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "InterActor.generated.h"

/**
 *
 */
UCLASS()
class AND_API AInterActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
public:
	AInterActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* Caller);
	
	virtual void OnInteract_Implementation(AActor* Caller) override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartFocus();
	
	virtual void StartFocus_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndFocus();
	
	virtual void EndFocus_Implementation() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	TArray<UMeshComponent*> Meshes;
};
