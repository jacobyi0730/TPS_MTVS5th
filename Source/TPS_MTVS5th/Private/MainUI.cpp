// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

void UMainUI::SetGameOverActive(bool bActive)
{
	GameOver->SetVisibility(
		bActive ?
		ESlateVisibility::Visible :
		ESlateVisibility::Hidden);
}
