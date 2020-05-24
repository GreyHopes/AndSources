// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class CharacterState : uint8 {
	PlayerState UMETA(DisplayName = "Player State"),
	GhostState UMETA(DisplayName = "Ghost State"),
};
