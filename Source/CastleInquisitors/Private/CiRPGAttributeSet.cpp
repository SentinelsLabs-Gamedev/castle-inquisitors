// Fill out your copyright notice in the Description page of Project Settings.


#include "CiRPGAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"

UCiRPGAttributeSet::UCiRPGAttributeSet()
{
	Health = 100.00f;
}

float UCiRPGAttributeSet::GetHealth() const
{
	return FMath::Max(Health.GetCurrentValue(), 0.0f);
}

float UCiRPGAttributeSet::SetHealth(float NewVal) const
{
	NewVal = FMath::Max(NewVal, 0.0f);

	if (UAbilitySystemComponent* Asc = GetOwningAbilitySystemComponent(); ensure(Asc))
	{
		Asc->SetNumericAttributeBase(GetHealthAttribute(), NewVal);
	}

	return NewVal;
}

void UCiRPGAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (const FGameplayAttribute ModifiedAttribute = Data.EvaluatedData.Attribute; ModifiedAttribute == GetHealthAttribute())
	{
		SetHealth(FMath::Max(GetHealth(), 0.0f));
	}
}

