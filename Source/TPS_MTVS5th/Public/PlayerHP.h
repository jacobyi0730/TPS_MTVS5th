// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHP.generated.h"

/**
 * 
 */
UCLASS()
class TPS_MTVS5TH_API UPlayerHP : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> HPBar;
	
	void SetHP(int32 curHP, int32 maxHP);
};
