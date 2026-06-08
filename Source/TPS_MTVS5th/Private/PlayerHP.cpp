// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHP.h"

#include "Components/ProgressBar.h"

// // 카운트 다운
// float curTime = 100;
//
// void Tick(float dt)
// {
// 	curTime -= dt;
// 	
// 	int32 sec = static_cast<int32>(curTime);
// 	int32 milliSec = static_cast<int32>(curTime * 100) % 100;
// }



void UPlayerHP::SetHP(int32 curHP, int32 maxHP)
{
	HPBar->SetPercent(static_cast<float>(curHP) / maxHP);
}
