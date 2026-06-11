// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPoolInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TPS_MTVS5TH_API IPoolInterface
{
	GENERATED_BODY()
public:
	// 풀에서 꺼내질 때 호출 (초기화, 이펙트 재생, Tick 동작 등)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSpwanFromPool();

	// 풀로 돌아갈 때 호출 (타이머 해제, Tick 정지, 리소스 해제 등)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnReturnToPool();
};
