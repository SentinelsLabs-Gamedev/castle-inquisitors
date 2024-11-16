#pragma once
#if WITH_GAMEPLAY_DEBUGGER

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

class APlayerController;
class AActor;

struct FRepData
{
	TMap<FString, FString> Data;

	void AddData(const FString& Key, const FString& Value)
	{
		Data.Add(Key, Value);
	}
	
	void PrintData(FGameplayDebuggerCanvasContext& CanvasContext) const
	{
		for (const auto& Elem : Data)
		{
			CanvasContext.Printf(TEXT("{yellow}%s: {white}%s"), *Elem.Key, *Elem.Value);
		}
	}

	void Serialize(FArchive& Ar);
};


class CI_FGameplayDebuggerCategory_Player: public FGameplayDebuggerCategory
{
public:
	CI_FGameplayDebuggerCategory_Player();
	virtual void CollectData(APlayerController* OwnerPC,AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;    
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

protected:
	FRepData DataPack;
};

#endif
