// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CiBaseCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "CiBaseAI.generated.h"

/**
 * 
 */
UCLASS()
class CASTLEINQUISITORS_API ACiBaseAI : public ACiBaseCharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;
	
public:
	ACiBaseAI();
};
