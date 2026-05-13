// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class TPS_MTVS5TH_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 외형과 충돌 컴포넌트를 생성하고싶다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyVar)
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyVar)
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	// 발사체 컴포넌트를 생성하고싶다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyVar)
	TObjectPtr<class UProjectileMovementComponent> MovementComp;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyVar)
	float SelfDestroyDelay = 1.f;
	
	void SelfDestroy();
};
