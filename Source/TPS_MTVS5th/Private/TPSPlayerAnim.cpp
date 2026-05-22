// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"

#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// 태어날 때 주인공을 가져오고싶다.
	Player = Cast<ATPSPlayer>(TryGetPawnOwner());
}

void UTPSPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// 만약 주인공이 유효하지 않으면 취소
	if (!Player || !Player->IsValidLowLevel())
		return;
	
	// 살아가면서 Speed와 Direction을 채우고싶다.
	FVector vel = Player->GetVelocity();
	
	Speed = FVector::DotProduct(Player->GetActorForwardVector(), vel);
	Direction = FVector::DotProduct(Player->GetActorRightVector(), vel);
	
	// 떨어지는 중인지 아닌지 계속 모니터링 하고싶다.
	bFalling = Player->GetCharacterMovement()->IsFalling();
	
	bCrouch = Player->GetCharacterMovement()->IsCrouching();
}

void UTPSPlayerAnim::PlayFireMontage()
{
	Montage_Play(FireMontage);
}

void UTPSPlayerAnim::PlayDiveRollMontage()
{
	Montage_Play(DiveRollMontage);
}
