// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComponent.h"

#include "Bullet.h"
#include "Enemy.h"
#include "EnhancedInputComponent.h"
#include "TPSPlayer.h"
#include "TPSPlayerAnim.h"
#include "TPSPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UPlayerFireComponent::UPlayerFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();

	OnMyChooseSniper(FInputActionValue());

	
}


// Called every frame
void UPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FOV가 ZoomTarget을 향해 보간처리되도록 하고싶다.
	if (Me->CameraComp && Me->CameraComp->IsValidLowLevel())
	{
		Me->CameraComp->FieldOfView = FMath::Lerp(Me->CameraComp->FieldOfView, ZoomTarget, DeltaTime * 10.f);
	}

}

void UPlayerFireComponent::SetPlayerInputComp(class UEnhancedInputComponent* input)
{
	Super::SetPlayerInputComp(input);
	
	input->BindAction(Me->IA_TPSFire, ETriggerEvent::Started, this, &UPlayerFireComponent::OnMyFire);

	input->BindAction(Me->IA_TPS1Key, ETriggerEvent::Started, this, &UPlayerFireComponent::OnMyChooseGun);

	input->BindAction(Me->IA_TPS2Key, ETriggerEvent::Started, this, &UPlayerFireComponent::OnMyChooseSniper);
		
	input->BindAction(Me->IA_TPSZoom, ETriggerEvent::Started, this, &UPlayerFireComponent::OnMyZoomIn);
	input->BindAction(Me->IA_TPSZoom, ETriggerEvent::Completed, this, &UPlayerFireComponent::OnMyZoomOut);

}


void UPlayerFireComponent::OnMyFire(const FInputActionValue& value)
{
	// 움찔하고싶다.
	if (auto* anim = Cast<UTPSPlayerAnim>(Me->GetMesh()->GetAnimInstance()))
	{
		anim->PlayFireMontage();
	}
	if (Me->FireSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Me->FireSound);
	}
	if (Me->FireCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(Me->FireCameraShake);
	}
	
	if (WeaponType == EWeaponType::SNIPER)
	{
		// 라인을 이용해서 총을 쏘고싶다.
		SharpShoot();
	}
	else
	{
		MakeBullet();
	}
}

void UPlayerFireComponent::OnMyChooseGun(const struct FInputActionValue& value)
{
	WeaponType = EWeaponType::GUN;
	// GunComp만 보이게하고싶다.
	Me->GunComp->SetVisibility(true);
	// SniperComp는 안보이게하고싶다.
	Me->SniperComp->SetVisibility(false);
	Me->PlayerCtrl->SetWeaponImage(WeaponType, EZoomType::ZOOM_OUT);
	ZoomTarget = 90.f;
}

void UPlayerFireComponent::OnMyChooseSniper(const struct FInputActionValue& value)
{
	WeaponType = EWeaponType::SNIPER;
	// SniperComp만 보이게하고싶다.
	Me->SniperComp->SetVisibility(true);
	// GunComp는 안보이게하고싶다.
	Me->GunComp->SetVisibility(false);
	Me->PlayerCtrl->SetWeaponImage(WeaponType,  EZoomType::ZOOM_OUT);
}

void UPlayerFireComponent::OnMyZoomIn(const struct FInputActionValue& value)
{
	// 만약 건이면 취소
	if (WeaponType == EWeaponType::GUN)
		return;
	
	ZoomTarget = 30.f;
	Me->PlayerCtrl->SetWeaponImage(WeaponType,  EZoomType::ZOOM_IN);
}

void UPlayerFireComponent::OnMyZoomOut(const struct FInputActionValue& value)
{
	if (WeaponType == EWeaponType::GUN)
		return;
	
	ZoomTarget = 90.f;
	Me->PlayerCtrl->SetWeaponImage(WeaponType,  EZoomType::ZOOM_OUT);
}



void UPlayerFireComponent::MakeBullet()
{
	FTransform t = Me->GunComp->GetSocketTransform(TEXT("FirePoint"));
	GetWorld()->SpawnActor<ABullet>(Me->BulletFactory, t);
}

void UPlayerFireComponent::SharpShoot()
{
	// 카메라 위치에서 카메라 앞방향으로 선을 쏴서 부딪힌 것이 있다면 타격을 하고 싶다.
	FHitResult OutHit;
	FVector Start = Me->CameraComp->GetComponentLocation();
	FVector End = Start + Me->CameraComp->GetForwardVector() * 100000.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Me);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility, Params);
	
	// 충돌한 물체가 있다면
	if (bHit)
	{
		// 충돌한 물체의 물리가 켜져있다면
		auto* hitComp = OutHit.GetComponent();
		if (hitComp && hitComp->IsSimulatingPhysics())
		{
			// 그 물체에게 힘을 가하고 싶다.
			FVector dir = OutHit.ImpactPoint - Start;
			FVector force = hitComp->GetMass() * dir.GetSafeNormal() * 5000.f;
			hitComp->AddImpulse(force);
		}
		else
		{
			// 총알자국을 표시하고싶다.
			FVector normalVector = OutHit.ImpactNormal;
			FVector dir = OutHit.ImpactPoint - Start;
			FRotator rot = UKismetMathLibrary::MakeRotFromZX(normalVector, dir.GetSafeNormal());
			
			GetWorld()->SpawnActor<AActor>(Me->BulletImpactFactory, OutHit.ImpactPoint, rot);
		}
		
		// 만약 충돌한 액터가 Enemy라면 Enemy에게 대미지를 전달하고싶다.
		auto* enemy = Cast<AEnemy>(OutHit.GetActor());
		if (enemy && enemy->IsValidLowLevel())
		{
			UGameplayStatics::ApplyDamage(
				enemy,
				1,
				Me->GetController(),
				Me,
				UDamageType::StaticClass()
				);
		}
	}
}


