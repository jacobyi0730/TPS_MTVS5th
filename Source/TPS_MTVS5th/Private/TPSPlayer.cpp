// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "Bullet.h"
#include "Enemy.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "TPSPlayerController.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoomComp = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoomComp"));
	CameraBoomComp->SetupAttachment(RootComponent);
	
	CameraBoomComp->TargetArmLength = 170.f;
	CameraBoomComp->SocketOffset = FVector(0.f, 80.f, 80.f); 
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(FName("CameraComp"));
	CameraComp->SetupAttachment(CameraBoomComp);
	
	GunComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("GunComp"));
	GunComp->SetupAttachment(GetMesh());
	GunComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SniperComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("SniperComp"));
	SniperComp->SetupAttachment(GetMesh());
	SniperComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}
	
	
	// 회전정보를 설정하고싶다.
	bUseControllerRotationYaw = true;
	CameraBoomComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false; 
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 키 매핑을 하고싶다.
	
	PlayerCtrl = Cast<ATPSPlayerController>(GetController());
	if (PlayerCtrl)
	{
		auto* subsys =ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerCtrl->GetLocalPlayer());
		
		if (subsys)
		{
			subsys->ClearAllMappings();
			subsys->AddMappingContext(IMC_TPSPlayer, 0);
		}
		// Pitch의 회전 각도를 제한하고싶다.
		PlayerCtrl->PlayerCameraManager->ViewPitchMin = -45.f;
		PlayerCtrl->PlayerCameraManager->ViewPitchMax = 45.f;
	}
	
	OnMyChooseGun(FInputActionValue());
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// FOV가 ZoomTarget을 향해 보간처리되도록 하고싶다.
	if (CameraComp && CameraComp->IsValidLowLevel())
	{
		CameraComp->FieldOfView = FMath::Lerp(CameraComp->FieldOfView, ZoomTarget, DeltaTime * 10.f);
	}
	
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	auto* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (input)
	{
		input->BindAction(IA_TPSMove, ETriggerEvent::Triggered, this, &ATPSPlayer::OnMyMove);
		
		input->BindAction(IA_TPSLook, ETriggerEvent::Triggered, this, &ATPSPlayer::OnMyLook);
		
		input->BindAction(IA_TPSJump, ETriggerEvent::Started, this, &ATPSPlayer::OnMyJump);
		
		input->BindAction(IA_TPSFire, ETriggerEvent::Started, this, &ATPSPlayer::OnMyFire);

		input->BindAction(IA_TPS1Key, ETriggerEvent::Started, this, &ATPSPlayer::OnMyChooseGun);

		input->BindAction(IA_TPS2Key, ETriggerEvent::Started, this, &ATPSPlayer::OnMyChooseSniper);
		
		input->BindAction(IA_TPSZoom, ETriggerEvent::Started, this, &ATPSPlayer::OnMyZoomIn);
		input->BindAction(IA_TPSZoom, ETriggerEvent::Completed, this, &ATPSPlayer::OnMyZoomOut);

	}
}

void ATPSPlayer::OnMyMove(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	FVector dir = FVector(v.X, v.Y, 0);
	dir = GetControlRotation().RotateVector(dir);
	AddMovementInput(dir, 1);
}

void ATPSPlayer::OnMyLook(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	
	AddControllerYawInput(v.X);
	AddControllerPitchInput(v.Y);
}

void ATPSPlayer::OnMyJump(const FInputActionValue& value)
{
	Jump();
}

void ATPSPlayer::OnMyFire(const FInputActionValue& value)
{
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

void ATPSPlayer::OnMyChooseGun(const struct FInputActionValue& value)
{
	WeaponType = EWeaponType::GUN;
	// GunComp만 보이게하고싶다.
	GunComp->SetVisibility(true);
	// SniperComp는 안보이게하고싶다.
	SniperComp->SetVisibility(false);
	PlayerCtrl->SetWeaponImage(WeaponType, EZoomType::ZOOM_OUT);
	ZoomTarget = 90.f;
}

void ATPSPlayer::OnMyChooseSniper(const struct FInputActionValue& value)
{
	WeaponType = EWeaponType::SNIPER;
	// SniperComp만 보이게하고싶다.
	SniperComp->SetVisibility(true);
	// GunComp는 안보이게하고싶다.
	GunComp->SetVisibility(false);
	PlayerCtrl->SetWeaponImage(WeaponType,  EZoomType::ZOOM_OUT);
}

void ATPSPlayer::OnMyZoomIn(const struct FInputActionValue& value)
{
	// 만약 건이면 취소
	if (WeaponType == EWeaponType::GUN)
		return;
	
	ZoomTarget = 30.f;
	PlayerCtrl->SetWeaponImage(WeaponType,  EZoomType::ZOOM_IN);
}

void ATPSPlayer::OnMyZoomOut(const struct FInputActionValue& value)
{
	if (WeaponType == EWeaponType::GUN)
		return;
	
	ZoomTarget = 90.f;
	PlayerCtrl->SetWeaponImage(WeaponType,  EZoomType::ZOOM_OUT);
}

void ATPSPlayer::MakeBullet()
{
	FTransform t = GunComp->GetSocketTransform(TEXT("FirePoint"));
	GetWorld()->SpawnActor<ABullet>(BulletFactory, t);
}

void ATPSPlayer::SharpShoot()
{
	// 카메라 위치에서 카메라 앞방향으로 선을 쏴서 부딪힌 것이 있다면 타격을 하고 싶다.
	FHitResult OutHit;
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 100000.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
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
			
			GetWorld()->SpawnActor<AActor>(BulletImpactFactory, OutHit.ImpactPoint, rot);
		}
		
		// 만약 충돌한 액터가 Enemy라면 Enemy에게 대미지를 전달하고싶다.
		auto* enemy = Cast<AEnemy>(OutHit.GetActor());
		if (enemy && enemy->IsValidLowLevel())
		{
			UGameplayStatics::ApplyDamage(
				enemy,
				1,
				GetController(),
				this,
				UDamageType::StaticClass()
				);
		}
	}
}

