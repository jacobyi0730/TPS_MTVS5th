// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(MYLOG, Log, All);

// 로그 출력 매크로 함수를 제작하고 싶다.
#define CALL_INFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__)  + TEXT(")"))
#define PRINT_CALL_INFO() UE_LOG(MYLOG, Warning, TEXT("%s"), *CALL_INFO)
#define PRINT_LOG(fmt, ...) \
	UE_LOG(MYLOG, Warning, TEXT("%s %s"), *CALL_INFO, *FString::Printf(fmt, ##__VA_ARGS__))