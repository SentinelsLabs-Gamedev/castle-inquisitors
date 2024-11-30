// Fill out your copyright notice in the Description page of Project Settings.


#include "CiBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "CiDamageEffect.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Abilities/CiGameplayTags.h"
#include "Abilities/CiMeleeAbility.h"

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

	if (UAbilitySystemComponent* Asc = GetAbilitySystemComponent(); IsValid(Asc))
	{
		AbilitySystemComponent->AddSet<UCiRPGAttributeSet>();
		RPGAttributeSet = Asc->GetSet<UCiRPGAttributeSet>();
		InitializeDefaultAbilities();
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
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(AbilitiesContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &ACiBaseCharacter::ActivateMeleeAbility);
	}
}

UAbilitySystemComponent* ACiBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACiBaseCharacter::InitializeDefaultAbilities()
{
	if (AbilitySystemComponent)
	{
		for (TSubclassOf Ability : DefaultAbilities)
		{
			if (Ability)
			{
				FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(
					FGameplayAbilitySpec(Ability, 1, INDEX_NONE));
				if (!AbilitySpecHandle.IsValid())
				{
					UE_LOG(LogTemp, Log, TEXT("InitializeDefaultAbilities failed"));
				}
			}
		}
	}
}

void ACiBaseCharacter::ActivateMeleeAbility()
{
	if (AbilitySystemComponent)
	{
		if (const bool bAbilitySuccess = AbilitySystemComponent->TryActivateAbilityByClass(
				UCiMeleeAbility::StaticClass()); !
			bAbilitySuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("ActivateAbility FAILED Called"));
		}
	}
}

void ACiBaseCharacter::TestHealthAttribute() const
{
	const TSubclassOf<UGameplayEffect> DamageEffectClass = UCiDamageEffect::StaticClass();
	if (!DamageEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("DamageEffectClass is null! Ensure it is properly set."));
		return;
	}

	const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName(EnumToDisplayName(ECiGameplayTags::DamageTest)));
	if (!DamageTag.IsValid())
	{
		UE_LOG(LogTemp, Error,
		       TEXT("Gameplay Tag is not valid! Ensure it is defined in the Gameplay Tag Table."));
		return;
	}

	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		DamageEffectClass, 1.0f, GetAbilitySystemComponent()->MakeEffectContext());
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

UAnimMontage* ACiBaseCharacter::GetMeleeMontage() const
{
	if (!MeleeMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("MeleeMontage is NULL for %s!"), *GetName());
		return nullptr;
	}
	return MeleeMontage;
}
