// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMoveComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPS_MTVS5TH_API UPlayerMoveComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void SetPlayerInputComp(class UEnhancedInputComponent* input) override;
	
	
	// 이동
	void OnMyMove(const struct FInputActionValue& value);
	void OnMyLook(const struct FInputActionValue& value);
	void OnMyJump(const struct FInputActionValue& value);
	void OnMyJog(const struct FInputActionValue& value);
	void OnMyWalk(const struct FInputActionValue& value);
	void OnMyCrouch(const struct FInputActionValue& value);
	void OnMyDiveRoll(const struct FInputActionValue& value);
};
