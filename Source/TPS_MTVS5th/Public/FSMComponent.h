// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../TPS_MTVS5th.h"
#include "FSMComponent.generated.h"



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPS_MTVS5TH_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFSMComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	EEnemyState State;
	void StateIdle();
	void StateMove();
	void StateAttack();
	void StateDamage();
	void StateDie();
	
	UPROPERTY()
	TObjectPtr<class ACharacter> Target;
	
	UPROPERTY()
	TObjectPtr<class AEnemy> Me;
	
	void OnMyTakeDamage(int32 damage);
	
	bool bDie;
	bool bAttack;
	float CurTime;
	float AttackDelayTime = 1.f;
	float ReactDelayTime = 1.f;
	float AttackDistance = 200.f;
	
	int32 CurHP;
	int32 MaxHP = 2;

	void OnMyAttackEnd();
	
	void SetState(EEnemyState newState);
	
	UPROPERTY()
	TObjectPtr<class UEnemyHPUI> HpUI;
	
	
	// 이동처리를 위해 AIController를 캐싱하고싶다.
	UPROPERTY()
	TObjectPtr<class AAIController> AI;
	
	FVector RandomTargetPoint;
	
	bool SetRandomTargetPoint(FVector& outTargetPoint);


};
