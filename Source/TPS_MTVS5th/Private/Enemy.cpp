// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "FSMComponent.h"
#include "NavigationInvokerComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HPComp = CreateDefaultSubobject<UWidgetComponent>(FName("HPComp"));
	HPComp->SetupAttachment(RootComponent);
	
	FSMComp = CreateDefaultSubobject<UFSMComponent>(FName("FSMComp"));
	
	NavInvokerComp = CreateDefaultSubobject<UNavigationInvokerComponent>(FName("NavInvokerComp"));
	
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// HPComp를 빌보딩하고싶다.
	// 카메라의 방향을 구해서 그 방향으로 HPComp를 회전하고싶다.
	FVector CamLoc = GetWorld()->GetFirstPlayerController()->
							PlayerCameraManager->GetCameraLocation();
	
	FVector dir = CamLoc - HPComp->GetComponentLocation();
	HPComp->SetWorldRotation(dir.ToOrientationRotator());
	
	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	FSMComp->OnMyTakeDamage(DamageAmount);
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

