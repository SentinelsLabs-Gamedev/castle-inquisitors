// Fill out your copyright notice in the Description page of Project Settings.


#include "CiBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "CiDamageEffect.h"

// Sets default values
ACiBaseCharacter::ACiBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	RPGAttributeSet = CreateDefaultSubobject<UCiRPGAttributeSet>(TEXT("RPGAttributeSet"));
}

// Called when the game starts or when spawned
void ACiBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UAbilitySystemComponent* Asc = GetAbilitySystemComponent();IsValid(Asc))
	{
		RPGAttributeSet = Asc->GetSet<UCiRPGAttributeSet>();
	}
}

// Called every frame
void ACiBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACiBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ACiBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACiBaseCharacter::TestHealthAttribute() const
{
	const TSubclassOf<UGameplayEffect> DamageEffectClass = UCiDamageEffect::StaticClass();
	if (!DamageEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("DamageEffectClass is null! Ensure it is properly set."));
		return;
	}

	const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.Test"));
	if (!DamageTag.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Gameplay Tag 'Damage.Test' is not valid! Ensure it is defined in the Gameplay Tag Table."));
		return;
	}

	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DamageEffectClass, 1.0f, GetAbilitySystemComponent()->MakeEffectContext());
	if (!EffectSpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("EffectSpecHandle is not valid!"));
		return;
	}

	
	if (FGameplayEffectSpec* Spec = EffectSpecHandle.Data.Get(); Spec)
	{
		Spec->SetSetByCallerMagnitude(DamageTag, -50.0f); 

		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve FGameplayEffectSpec from EffectSpecHandle!"));
	}
}

const UCiRPGAttributeSet* ACiBaseCharacter::GetRPGAttributeSet() const
{
	return this->RPGAttributeSet;
}

