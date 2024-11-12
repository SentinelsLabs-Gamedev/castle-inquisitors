#include "CI_FGameplayDebuggerCategory_Player.h"

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
		DataPack.AddData(TEXT("Actor Name"), OwnerPC->GetPawn()->GetName());
		DataPack.AddData(TEXT("Actor Position"), OwnerPC->GetPawn()->GetActorLocation().ToString());
		DataPack.AddData(TEXT("Actor Rotation"), OwnerPC->GetPawn()->GetActorRotation().ToString());
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
