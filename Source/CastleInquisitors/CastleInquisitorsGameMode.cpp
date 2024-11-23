// Copyright Epic Games, Inc. All Rights Reserved.

#include "CastleInquisitorsGameMode.h"

#include "UWB_Fade.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACastleInquisitorsGameMode::ACastleInquisitorsGameMode(): CastleInquisitorsUI(nullptr)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/Gameplay/Blueprints/BP_CI_Knight_Character"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACastleInquisitorsGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

// This implementation runs when the PlayerController + LevelAssets are ready
void ACastleInquisitorsGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	if (!CastleInquisitorsUI)
	{
		if (UClass* FadeClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Gameplay/Blueprints/Widgets/Fade_Widget.Fade_Widget_C")))
		{
			CastleInquisitorsUI = CreateWidget<UUserWidget>(GetWorld(), FadeClass);
			if (CastleInquisitorsUI)
			{
				CastleInquisitorsUI->AddToViewport();
				UE_LOG(LogTemp, Log, TEXT("CastleInquisitorsUI added to viewport."));
                
				if (UWb_Fade* FadeWidget = Cast<UWb_Fade>(CastleInquisitorsUI))
				{
					FadeWidget->PlayFadeOutAnimation();
				}
			}
		}
	}
}