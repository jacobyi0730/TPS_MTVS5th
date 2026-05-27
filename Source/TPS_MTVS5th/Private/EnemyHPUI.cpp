// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHPUI.h"

#include "Components/ProgressBar.h"

void UEnemyHPUI::UpdateHPBar(float cur, float max)
{
	HPBar->SetPercent(cur / max);
}
