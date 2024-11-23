#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWB_Fade.generated.h"

UCLASS()
class CASTLEINQUISITORS_API UWb_Fade : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void PlayFadeInAnimation();

	UFUNCTION(BlueprintCallable, Category = "Fade")
	void PlayFadeOutAnimation();
	
protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeInAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutAnimation;
};
