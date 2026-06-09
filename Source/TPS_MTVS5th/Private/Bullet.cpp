// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "ObjectPoolSubSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SetRootComponent(SphereComp);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");

	// 충돌 설정을 하고싶다.
	SphereComp->SetCollisionProfileName(TEXT("BlockAll"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	// 수명을 주고 싶다.
	//SetLifeSpan(10.f);
	FTimerHandle handle;
	//GetWorldTimerManager().SetTimer(handle, this, &ABullet::SelfDestroy, SelfDestroyDelay);
	
	// Lambda(대단히소중하다), 익명함수
	int k = 10;
	auto myAdd = [&](int32 a, int32 b)->int32
	{
		return a + b;
	};
	
	int32 result = myAdd(10, 1909);
		
	GetWorldTimerManager().SetTimer(handle, [&]()
	{
		MovementComp->Deactivate();
		MovementComp->SetComponentTickEnabled(false);
		MovementComp->Velocity = FVector::ZeroVector;
		MovementComp->UpdateComponentVelocity();
		
		auto* pool = GetWorld()->GetSubsystem<UObjectPoolSubSystem>();
		pool->ReturnToPool(this);

	}, SelfDestroyDelay, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::ResetMovementComp()
{
	MovementComp->SetUpdatedComponent(RootComponent);
	MovementComp->Velocity = GetActorForwardVector() * MovementComp->InitialSpeed;
	MovementComp->SetComponentTickEnabled(true);
	MovementComp->Activate(true);
}

void ABullet::SelfDestroy()
{
	Destroy();
}

