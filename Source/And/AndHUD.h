// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AndHUD.generated.h"

UCLASS()
class AAndHUD : public AHUD
{
	GENERATED_BODY()

public:
	AAndHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;
};

