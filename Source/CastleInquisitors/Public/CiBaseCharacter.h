// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CiRPGAttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "CiBaseCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CASTLEINQUISITORS_API ACiBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Abilities", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Attributes", meta = (AllowPrivateAccess = "true"))
	const UCiRPGAttributeSet* RPGAttributeSet;

	// DefaultAbilities are assigned through Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability", meta=(AllowPrivateAccess=true))
	UAnimMontage* MeleeMontage;
	
	
public:
	// Sets default values for this character's properties
	ACiBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category="Functions")
	void TestHealthAttribute() const;

	// Returns our ability system component
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void InitializeDefaultAbilities();

	void ActivateMeleeAbility();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	const UCiRPGAttributeSet* GetRPGAttributeSet() const;

	UAnimMontage* GetMeleeMontage() const;
};
