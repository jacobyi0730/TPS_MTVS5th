// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_MTVS5TH_API UTPSPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY()
	TObjectPtr<class ATPSPlayer> Player;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MyVar")
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MyVar")
	float Direction;
};
