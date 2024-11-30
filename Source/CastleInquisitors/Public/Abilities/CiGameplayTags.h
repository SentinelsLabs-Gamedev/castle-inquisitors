#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECiGameplayTags : uint8
{
	DamageTest UMETA(DisplayName = "Damage.Test"),
};

FString EnumToDisplayName(ECiGameplayTags GameplayTag);