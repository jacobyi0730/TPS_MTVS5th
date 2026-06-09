// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubSystem.generated.h"

USTRUCT(BlueprintType)
struct FPoolActorArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<AActor*> PooledActors;
};

/**
 * 
 */
UCLASS()
class TPS_MTVS5TH_API UObjectPoolSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FPoolActorArray> ObjectPools;
	
	// 초기화 :  Pool에 미리 객체를 만들어 놓는 기능
	void InitPool(TSubclassOf<AActor> factory, int32 initSize);
	// 꺼내기
	AActor* SpawnFromPool(TSubclassOf<AActor> factory, FVector location, FRotator rotation);
	AActor* SpawnFromPool(TSubclassOf<AActor> factory, FTransform t);
	// 돌려놓기
	void ReturnToPool(AActor* actor);
	
	void SetActive(AActor* actor, bool bActive);
	
	virtual void Deinitialize() override;
};
