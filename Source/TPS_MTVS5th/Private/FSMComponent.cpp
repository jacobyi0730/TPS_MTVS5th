// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMComponent.h"

#include "Enemy.h"
#include "TPS_MTVS5th/TPS_MTVS5th.h"


// Sets default values for this component's properties
UFSMComponent::UFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	Me = Cast<AEnemy>(GetOwner());
	
	CurHP = MaxHP;
	
}


// Called every frame
void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (State)
	{
	case EEnemyState::IDLE:		StateIdle();		break;
	case EEnemyState::MOVE:		StateMove();		break;
	case EEnemyState::ATTACK:	StateAttack();		break;
	case EEnemyState::DAMAGE:	StateDamage();		break;
	case EEnemyState::DIE:		StateDie();			break;
	}
	FVector loc = Me->GetActorLocation() + FVector(0, 0, 50.f);
	FString stateStr = UEnum::GetValueAsString(State);
	DrawDebugString(GetWorld(), loc, stateStr, nullptr, FColor::Cyan, 0);
	
}

void UFSMComponent::StateIdle()
{
	// TODO : 주인공을 찾고싶다.
	// 조건(Condition) : 주인공을 찾았다면
	Target = GetWorld()->GetFirstPlayerController()->GetCharacter();
	// 전이(Transition) : 이동 상태
	if (nullptr != Target)
	{
		State = EEnemyState::MOVE;
	}
}

void UFSMComponent::StateMove()
{
	// TODO : Target을 향해 이동하고 싶다.
	FVector dir = Target->GetActorLocation() - Me->GetActorLocation();
	Me->AddMovementInput(dir, 1);
	
	float distance = dir.Size();
	// 조건 : Target과의 거리가 2미터 이내라면
	if (distance <= 200.f)
	{
		// 전이 : 공격 상태로 전이하고싶다.
		State = EEnemyState::ATTACK;
	}
}

void UFSMComponent::StateAttack()
{
	// sub공격과 sub공격대기 상태를 구현하고싶다.
	if (bAttack)
	{
		// 조건 : 타겟이 공격가능거리를 벗어났다면
		float dist = Me->GetDistanceTo(Target);
		if (dist > AttackDistance)
		{
			// 이동상태로 전이하고싶다.
			State = EEnemyState::MOVE;
			return;	
		}

		// 공격!
		PRINT_LOG(TEXT("Attack!!!!!"));
		// sub공격대기 상태로 전이하고싶다.
		bAttack = false;
	}
	else
	{
		// 시간이 흐르다가
		CurTime += GetWorld()->GetDeltaSeconds();
		// 만약 현재시간이 sub공격대기시간을 초과하면
		if (CurTime > AttackDelayTime)
		{
			// 현재시간을 0으로 초기화하고
			CurTime = 0;
			// sub공격상태로 전이하고싶다.
			bAttack = true;
		}
	}
	
}

void UFSMComponent::StateDamage()
{
	// 시간이 흐르다가 
	CurTime += GetWorld()->GetDeltaSeconds();
	// 현재시간이 리액션시간(ReactDelayTime)을 초과하면
	if (CurTime > ReactDelayTime)
	{
		CurTime = 0;
		// 이동상태로 전이하고싶다.
		State = EEnemyState::MOVE;
	}
}

void UFSMComponent::StateDie()
{
}

void UFSMComponent::OnMyTakeDamage(int32 damage)
{
	// 현재 상태가 죽거나 데미지라면 취소
	if (State == EEnemyState::DIE || State == EEnemyState::DAMAGE)
	{
		return;
	}
	
	CurHP -= damage;
	CurTime = 0;
	if (CurHP <= 0.f)
	{
		State = EEnemyState::DIE;
	}
	else
	{
		State = EEnemyState::DAMAGE;
	}
}

