// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"

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
}

void ATPSPlayerController::SetWeaponImage(EWeaponType type, EZoomType zoom)
{
	WeaponUI->ShowImage(type, zoom);
}
