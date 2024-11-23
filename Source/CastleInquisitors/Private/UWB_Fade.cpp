#include "UWB_Fade.h"

void UWb_Fade::PlayFadeInAnimation()
{
	if (FadeInAnimation)
	{
		PlayAnimation(FadeInAnimation);
	}
}
void UWb_Fade::PlayFadeOutAnimation()
{
	if (FadeOutAnimation)
	{
		PlayAnimation(FadeOutAnimation);
	}
}