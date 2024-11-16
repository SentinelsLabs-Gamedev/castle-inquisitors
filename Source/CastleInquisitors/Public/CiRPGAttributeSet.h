// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "CiRPGAttributeSet.generated.h"

UCLASS()
class CASTLEINQUISITORS_API UCiRPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCiRPGAttributeSet();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;

public:
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UCiRPGAttributeSet, Health);
	float GetHealth() const;
	float SetHealth(float NewVal) const;
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Health);

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};

