// Copyright Epic Games, Inc. All Rights Reserved.

#include "CastleInquisitors.h"
#include "Modules/ModuleManager.h"

#if WITH_GAMEPLAY_DEBUGGER
	#include "GameplayDebugger.h"
	#include "Debugger/CI_FGameplayDebuggerCategory_Player.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogCastleInquisitors, Log, All);

class FCastleInquisitorsModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
		UE_LOG(LogCastleInquisitors, Log, TEXT("FCastleInquisitorsModule has started up"));
		#if WITH_GAMEPLAY_DEBUGGER
		UE_LOG(LogCastleInquisitors, Log, TEXT("Registering Gameplay Debugger Category"));
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.RegisterCategory(
			"Player",
			IGameplayDebugger::FOnGetCategory::CreateStatic(&CI_FGameplayDebuggerCategory_Player::MakeInstance),
			EGameplayDebuggerCategoryState::EnabledInGameAndSimulate,
			6
		);
		GameplayDebuggerModule.NotifyCategoriesChanged();
		#endif
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(LogCastleInquisitors, Log, TEXT("FCastleInquisitorsModule is shutting down"));
		#if WITH_GAMEPLAY_DEBUGGER
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("Player");
		GameplayDebuggerModule.NotifyCategoriesChanged();
		#endif // WITH_GAMEPLAY_DEBUGGER
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FCastleInquisitorsModule, CastleInquisitors, "CastleInquisitors");