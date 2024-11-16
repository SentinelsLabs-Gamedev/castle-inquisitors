// Fill out your copyright notice in the Description page of Project Settings.


#include "CiDamageEffect.h"

#include "CiRPGAttributeSet.h"

UCiDamageEffect::UCiDamageEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayModifierInfo ModifiedInfo;
	ModifiedInfo.Attribute = UCiRPGAttributeSet::GetHealthAttribute();
	ModifiedInfo.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat SetByCallerMagnitude;
	SetByCallerMagnitude.DataTag = FGameplayTag::RequestGameplayTag(FName("Damage.Test"));
	ModifiedInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerMagnitude);
	
	Modifiers.Add(ModifiedInfo);
}
