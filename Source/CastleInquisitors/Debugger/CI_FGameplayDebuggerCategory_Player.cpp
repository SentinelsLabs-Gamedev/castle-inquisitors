#include "CI_FGameplayDebuggerCategory_Player.h"

#include "CastleInquisitors/CastleInquisitorsCharacter.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"


CI_FGameplayDebuggerCategory_Player::CI_FGameplayDebuggerCategory_Player()
{
	SetDataPackReplication(&DataPack);
}

void CI_FGameplayDebuggerCategory_Player::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	if (OwnerPC && OwnerPC->GetPawn())
	{
		auto OwnerPawn = OwnerPC->GetPawn();
		auto Character = Cast<ACastleInquisitorsCharacter>(OwnerPawn);
		DataPack.AddData(TEXT("Actor Name"), OwnerPawn->GetName());
		DataPack.AddData(TEXT("Actor Position"), OwnerPawn->GetActorLocation().ToString());
		DataPack.AddData(TEXT("Actor Rotation"), OwnerPawn->GetActorRotation().ToString());
		DataPack.AddData(TEXT("Actor Health"),FString::SanitizeFloat(Character -> GetRPGAttributeSet() -> GetHealth()));
	}
}

void CI_FGameplayDebuggerCategory_Player::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	DataPack.PrintData(CanvasContext);
}

TSharedRef<FGameplayDebuggerCategory> CI_FGameplayDebuggerCategory_Player::MakeInstance()
{
return MakeShareable(new CI_FGameplayDebuggerCategory_Player());
}

void FRepData::Serialize(FArchive& Ar)
{
	Ar << Data;
}



#endif
