// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPS_MTVS5th/TPS_MTVS5th.h"
#include "TPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TPS_MTVS5TH_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
	virtual void BeginPlay() override;
	void SetWeaponImage(EWeaponType type, EZoomType zoom);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MyVar)
	TSubclassOf<class UWeaponUI> WeaponUIFactory;
	
	UPROPERTY()
	TObjectPtr<UWeaponUI> WeaponUI;
	
};
