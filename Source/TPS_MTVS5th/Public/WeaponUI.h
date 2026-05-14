// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPS_MTVS5th/TPS_MTVS5th.h"
#include "WeaponUI.generated.h"

/**
 * 
 */
UCLASS()
class TPS_MTVS5TH_API UWeaponUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ImageCrosshair;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ImageScope;
	
	void ShowImage(EWeaponType type, EZoomType zoom);
};
