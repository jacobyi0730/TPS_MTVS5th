// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "TPS_MTVS5th/TPS_MTVS5th.h"
#include "PlayerFireComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPS_MTVS5TH_API UPlayerFireComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerFireComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual  void SetPlayerInputComp(class UEnhancedInputComponent* input) override;
	
	
	EWeaponType WeaponType;
	float ZoomTarget = 90.f;

	void OnMyFire(const struct FInputActionValue& value);
	void OnMyChooseGun(const struct FInputActionValue& value);
	void OnMyChooseSniper(const struct FInputActionValue& value);
	void OnMyZoomIn(const struct FInputActionValue& value);
	void OnMyZoomOut(const struct FInputActionValue& value);
	
	void MakeBullet();
	void SharpShoot();

};
