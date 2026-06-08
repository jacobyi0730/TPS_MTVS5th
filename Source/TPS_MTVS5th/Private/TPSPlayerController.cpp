// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"

#include "MainUI.h"
#include "WeaponUI.h"
#include "Blueprint/UserWidget.h"

void ATPSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// WeaponUIFactory를 이용해서 UWeaponUI 생성해서 WeaponUI에 담고 Viewport에 붙이세요.

	WeaponUI = CastChecked<UWeaponUI>(CreateWidget(GetWorld(), WeaponUIFactory));
	if (WeaponUI)
	{
		WeaponUI->AddToViewport();
	}

	MainUI = CastChecked<UMainUI>(CreateWidget(GetWorld(), MainUIFactory));
	if (MainUI)
	{
		MainUI->AddToViewport();
	}

}

void ATPSPlayerController::SetWeaponImage(EWeaponType type, EZoomType zoom)
{
	if (WeaponUI && WeaponUI->IsValidLowLevel())
	{
		WeaponUI->ShowImage(type, zoom);
	}
}
