// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CiBaseAI.h"

ACiBaseAI::ACiBaseAI()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	PawnSensingComponent->SetPeripheralVisionAngle(25.0f);
	PawnSensingComponent->SightRadius = 2000.0f;
}
