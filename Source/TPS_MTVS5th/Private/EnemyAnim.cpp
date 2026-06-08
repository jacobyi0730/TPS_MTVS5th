// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

#include "Enemy.h"
#include "FSMComponent.h"
#include "TPSPlayer.h"

void UEnemyAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// 태어날 때 나를 소유한 오너를 Enemy에 기억하고싶다.
	Enemy = Cast<AEnemy>(TryGetPawnOwner());
}

void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// 살아가면서 그 오너의 상태와 공격여부를 동기화 하고싶다.
	if (Enemy && Enemy->IsValidLowLevel() &&
		Enemy->FSMComp && Enemy->FSMComp->IsValidLowLevel())
	{
		State = Enemy->FSMComp->State;
		bAttack = Enemy->FSMComp->bAttack;
	}
}

void UEnemyAnim::AnimNotify_AttackEnd()
{
	//PRINT_LOG(TEXT("AnimNotify_AttackEnd"));
	if (Enemy && Enemy->IsValidLowLevel() &&
		Enemy->FSMComp && Enemy->FSMComp->IsValidLowLevel())
	{
		Enemy->FSMComp->OnMyAttackEnd();
	}
}

void UEnemyAnim::AnimNotify_Hit()
{
	//PRINT_LOG(TEXT("AnimNotify_Hit"));
	// 주인공에게 데미지를 가하고싶다.

	// sub공격대기 상태로 전이하고싶다.
	if (auto fsm = GetFSMComp())
	{
		fsm->bAttack = false;
		
		auto* target = GetWorld()->GetFirstPlayerController()->GetCharacter();
		// 주인공이 공격 거리에 있다면?
		float dist = Enemy->GetDistanceTo(target);
		if (dist < 150.f)
		{
			// 주인공이 enemy의 전방에 있다면?
			// 공격!
			if (auto* player = Cast<ATPSPlayer>(target))
			{
				player->DoDamage(1);
			}
		}

	}
}

void UEnemyAnim::AnimNotify_DamageEnd()
{
	Montage_Stop(0, EnemyMontage);

	if (auto fsm = GetFSMComp())
	{
		fsm->SetState(EEnemyState::MOVE);
	}
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	// 이제 바닥으로 내려가도된다고하고싶다.
	if (auto fsm = GetFSMComp())
	{
		fsm->bDie = true;
	}
}

void UEnemyAnim::PlayDamageMontage(int32 idx)
{
	FString sectionName = FString::Printf(TEXT("Damage%d"), idx);

	//Enemy->PlayAnimMontage(EnemyMontage, 0, FName(*sectionName));

	Montage_Play(EnemyMontage);
	Montage_JumpToSection(FName(*sectionName), EnemyMontage);
}

void UEnemyAnim::PlayDieMontage()
{
	FString sectionName = TEXT("Die");

	Enemy->PlayAnimMontage(EnemyMontage, 1, TEXT("Die"));
	
	// Montage_Play(EnemyMontage);
	// Montage_JumpToSection(FName(*sectionName), EnemyMontage);
}

TObjectPtr<UFSMComponent> UEnemyAnim::GetFSMComp()
{
	if (Enemy && Enemy->IsValidLowLevel() &&
		Enemy->FSMComp && Enemy->FSMComp->IsValidLowLevel())
	{
		return Enemy->FSMComp;
	}
	return nullptr;
}
