// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSubSystem.h"

#include "PoolInterface.h"

void UObjectPoolSubSystem::InitPool(TSubclassOf<AActor> factory, int32 initSize)
{
	if (nullptr == factory || 0 == initSize)
	{
		return;
	}
	
	for (int32 i = 0; i < initSize; i++)
	{
		if (AActor* actor = GetWorld()->SpawnActor<AActor>(factory, FVector::ZeroVector, FRotator::ZeroRotator))
		{
			ReturnToPool(actor);
		}
	}
}

AActor* UObjectPoolSubSystem::SpawnFromPool(TSubclassOf<AActor> factory, FVector location, FRotator rotation)
{
	if (nullptr == factory) return nullptr;
	
	AActor* retActor = nullptr; 
	
	// ObjectPools에 factory가 있는지? 값이 존재하는지?
	if (ObjectPools.Contains(factory) && ObjectPools[factory].PooledActors.Num() > 0)
	{
		retActor = ObjectPools[factory].PooledActors.Pop();
	}
	else
	{
		// 풀이 비어있으면 새로 생성해서 반환
		retActor = GetWorld()->SpawnActor<AActor>(factory, FVector::ZeroVector, FRotator::ZeroRotator);
	}
	// 초기화 처리
	retActor->SetActorLocationAndRotation(location, rotation);
	
	if (retActor->Implements<UPoolInterface>())
	{
		IPoolInterface::Execute_OnSpwanFromPool(retActor);
	}
	
	SetActive(retActor, true);
	
	return retActor;
}

AActor* UObjectPoolSubSystem::SpawnFromPool(TSubclassOf<AActor> factory, FTransform t)
{
	return SpawnFromPool(factory, t.GetLocation(), t.GetRotation().Rotator());
}

void UObjectPoolSubSystem::ReturnToPool(AActor* actor)
{
	if (nullptr == actor) return;
	
	if (actor->Implements<UPoolInterface>())
	{
		IPoolInterface::Execute_OnReturnToPool(actor);
	}
	
	SetActive(actor, false);
	
	ObjectPools.FindOrAdd(actor->GetClass()).PooledActors.Add(actor);
}

void UObjectPoolSubSystem::SetActive(AActor* actor, bool bActive)
{
	// 틱 처리
	actor->SetActorTickEnabled(bActive);
	// 그리기 처리
	actor->SetActorHiddenInGame(!bActive);
	// 충돌 처리
	actor->SetActorEnableCollision(bActive);
}

void UObjectPoolSubSystem::Deinitialize()
{
	ObjectPools.Empty();
	Super::Deinitialize();
}
