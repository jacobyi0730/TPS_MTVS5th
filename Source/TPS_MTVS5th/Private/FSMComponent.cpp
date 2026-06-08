// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMComponent.h"

#include "TPS_MTVS5th/TPS_MTVS5th.h"
#include "Enemy.h"
#include "EnemyAnim.h"
#include "EnemyHPUI.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationSystemTypes.h"
#include "TPSPlayer.h"
#include "Navigation/PathFollowingComponent.h"


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
	
	AI = Cast<AAIController>(Me->GetController());
	
	HpUI = Cast<UEnemyHPUI>(Me->HPComp->GetWidget());
	
	CurHP = MaxHP;
	if (HpUI)
	{
		HpUI->UpdateHPBar(1.f, 1.f);
	}
	
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
	FVector destination = Target->GetActorLocation();
	FVector dir = destination - Me->GetActorLocation();
	//Me->AddMovementInput(dir, 1);
	
	// 순찰, 추적을 구현하고 싶다.
	// 만약 Target이 길 위에 있다면 추적을 하고싶고
	auto* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetAcceptanceRadius(150.f);
	req.SetGoalLocation(destination);
	
	AI->BuildPathfindingQuery(req, query);
	
	auto result = ns->FindPathSync(query);
	// 만약 Target이 길 위에 있다면
	if (result.IsSuccessful())
	{
		AI->MoveToLocation(destination);
	}
	// 그렇지않다면 순찰을 하고싶다. 
	else
	{
		//	순찰을 하기위해 길위의 랜덤한 위치를 정해서 그곳으로 이동하고싶다.
		auto res = AI->MoveToLocation(RandomTargetPoint);
		// 만약 이미도착했거나 실패했다면
		if (res != EPathFollowingRequestResult::Type::RequestSuccessful)
		{
			// 다시 목적지를 만들어주고싶다.
			SetRandomTargetPoint(RandomTargetPoint);
		}
	}
	
	
	

	
	
	
	
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

	}
}

void UFSMComponent::OnMyAttackEnd()
{
	// 현재시간을 0으로 초기화하고
	CurTime = 0;
	// sub공격상태로 전이하고싶다.
	bAttack = true;
}

void UFSMComponent::SetState(EEnemyState newState)
{
	CurTime = 0;
	State = newState;
	
	SetRandomTargetPoint(RandomTargetPoint);
}

bool UFSMComponent::SetRandomTargetPoint(FVector& outTargetPoint)
{
	// 갈 수 있는 길위의 랜덤한 위치를 기억하고싶다.
	
	auto* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FVector origin = Me->GetActorLocation();
	float radius = 500.f;
	FNavLocation resultLoc;
	if (ns->GetRandomReachablePointInRadius(origin, radius, resultLoc))
	{
		outTargetPoint = resultLoc.Location;
		return true;
	}
	
	return false;
}


void UFSMComponent::StateDamage()
{
	// // 시간이 흐르다가 
	// CurTime += GetWorld()->GetDeltaSeconds();
	// // 현재시간이 리액션시간(ReactDelayTime)을 초과하면
	// if (CurTime > ReactDelayTime)
	// {
	// 	CurTime = 0;
	// 	// 이동상태로 전이하고싶다.
	// 	State = EEnemyState::MOVE;
	// }
}

void UFSMComponent::StateDie()
{
	if (!bDie)
	{
		return;
	}
	
	// 시간이 흐르다가
	CurTime += GetWorld()->GetDeltaSeconds();
	
	// 바닥으로 내려가고싶다.
	Me->SetActorLocation(Me->GetActorLocation() + FVector(0, 0, -200) * GetWorld()->GetDeltaSeconds());
	
	// 2초가 지나면 파괴되고싶다.
	if (CurTime > 2.f)
	{
		Me->Destroy();
	}
}

void UFSMComponent::OnMyTakeDamage(int32 damage)
{
	// 현재 상태가 죽거나 데미지라면 취소
	if (State == EEnemyState::DIE || State == EEnemyState::DAMAGE)
	{
		return;
	}
	
	AI->StopMovement();
	
	CurHP -= damage;
	if (HpUI)
	{
		HpUI->UpdateHPBar(static_cast<float>(CurHP), static_cast<float>(MaxHP));
	}
	CurTime = 0;
	if (CurHP <= 0.f)
	{
		State = EEnemyState::DIE;
		// 캡슐의 충돌체를 끄고싶다.
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		auto* anim = Cast<UEnemyAnim>(Me->GetMesh()->GetAnimInstance());
		anim->PlayDieMontage();
	}
	else
	{
		State = EEnemyState::DAMAGE;
		// 나의 오너가 가지고있는 Mesh를 알고싶고,
		// 그 Mesh에게 AnimInstace를 가져와서
		// AnimInstance를 EnemyAnim으로 캐스팅 하고싶다.
		auto* anim = Cast<UEnemyAnim>(Me->GetMesh()->GetAnimInstance());
		
		int32 randValue = FMath::RandRange(0, 1);
		anim->PlayDamageMontage(randValue);
	}
}


