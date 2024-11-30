// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CiMeleeAbility.generated.h"

/**
 * 
 */
UCLASS()
class CASTLEINQUISITORS_API UCiMeleeAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCiMeleeAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	void PerformMelee();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayEffect> DamageEffect;
};
