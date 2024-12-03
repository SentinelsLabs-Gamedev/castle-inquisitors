#include "Abilities/CiGameplayTags.h"

FString EnumToDisplayName(ECiGameplayTags GameplayTag)
{
	return StaticEnum<ECiGameplayTags>()->GetDisplayNameTextByIndex(static_cast<int32>(GameplayTag)).ToString();
}
