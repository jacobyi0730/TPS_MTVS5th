// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

#include "Enemy.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyManager::AEnemyManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnPoints.Empty(5);
	// 레벨에서 tag이 SpawnPoint인 StaticMeshActor를 모두 찾고싶다.
	// 찾은 녀석들을 SpawnPoints에 넣고싶다.
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStaticMeshActor::StaticClass(), FName("SpawnPoint"), SpawnPoints);
	
	
	GetWorldTimerManager().SetTimer(TimerHandle, [&]()
	{
		MakeEnemy();
	}, 2, true);
}

void AEnemyManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AEnemyManager::MakeEnemy()
{
	// SpawnPoints 중에 랜덤으로 하나 정해서 t를 확정하고싶다.
	int32 index = index = FMath::RandRange(0, SpawnPoints.Num() - 1);
	if (index == LatestIndex)
	{
		index = (index + 1) % SpawnPoints.Num();
	}
	LatestIndex = index;
	FTransform t = SpawnPoints[index]->GetActorTransform();
	
	GetWorld()->SpawnActor<AEnemy>(EnemyFactory, t);
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


