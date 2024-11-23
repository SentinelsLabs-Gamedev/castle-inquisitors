// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CastleInquisitorsGameMode.generated.h"

UCLASS(minimalapi)
class ACastleInquisitorsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACastleInquisitorsGameMode();
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

private:
	UPROPERTY()
	UUserWidget* CastleInquisitorsUI;
};





