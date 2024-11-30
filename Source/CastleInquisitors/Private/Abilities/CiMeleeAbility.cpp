// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CiMeleeAbility.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemGlobals.h"
#include "CiBaseCharacter.h"
#include "CiDamageEffect.h"
#include "Abilities/CiGameplayTags.h"

UCiMeleeAbility::UCiMeleeAbility()
{
	DamageEffect = UCiDamageEffect::StaticClass();
}

void UCiMeleeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (ActorInfo->AvatarActor.IsValid())
	{
		ACiBaseCharacter* Character = Cast<ACiBaseCharacter>(ActorInfo->AvatarActor.Get());
		if (Character && Character->GetMesh())
		{
			Character->PlayAnimMontage(Character->GetMeleeMontage());
		}
	}

	PerformMelee();

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UCiMeleeAbility::PerformMelee()
{
	AActor* OwnerActor = GetOwningActorFromActorInfo();
	if (!OwnerActor) return;

	FVector StartLocation = OwnerActor->GetActorLocation();
	FVector ForwardVector = OwnerActor->GetActorForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * 200.0f);

	float CapsuleRadius = 50.0f;
	float CapsuleHalfHeight = 100.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerActor);

	TArray<FHitResult> HitResults;

	const bool bHit = GetWorld()->SweepMultiByObjectType(HitResults, StartLocation, EndLocation, FQuat::Identity,
	                                                     ObjectParams,
	                                                     FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
	                                                     Params);
	if (bHit)
	{
		const FHitResult& FirstHit = HitResults[0];
		AActor* HitActor = FirstHit.GetActor();

		if (HitActor && DamageEffect)
		{
			if (UAbilitySystemComponent* TargetASC =
				UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitActor))
			{
				FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
				EffectContext.AddSourceObject(this);

				FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffect, 1.0f, EffectContext);
				if (SpecHandle.IsValid())
				{
					if (FGameplayEffectSpec* Spec = SpecHandle.Data.Get(); Spec)
					{
						const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName(EnumToDisplayName(ECiGameplayTags::DamageTest)));
						Spec->SetSetByCallerMagnitude(DamageTag, -10.0f);
						TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);	
					}
				}
			}
		}
	}
}
