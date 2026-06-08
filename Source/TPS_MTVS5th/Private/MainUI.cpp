// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UMainUI::SetGameOverActive(bool bActive)
{
	GameOver->SetVisibility(
		bActive ?
		ESlateVisibility::Visible :
		ESlateVisibility::Hidden);
}

void UMainUI::SetChooseGun(bool bGun)
{
	Cast<UCanvasPanelSlot>(ImageGun->Slot)->SetZOrder(bGun ? 2 : 0); 
	Cast<UCanvasPanelSlot>(ImageSniper->Slot)->SetZOrder(bGun ? 0 : 2);
}
