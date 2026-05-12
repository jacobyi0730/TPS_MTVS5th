// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "Bullet.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


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
	
	auto* pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		auto* subsys =ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		
		if (subsys)
		{
			subsys->ClearAllMappings();
			subsys->AddMappingContext(IMC_TPSPlayer, 0);
		}
		// Pitch의 회전 각도를 제한하고싶다.
		pc->PlayerCameraManager->ViewPitchMin = -45.f;
		pc->PlayerCameraManager->ViewPitchMax = 45.f;
	}
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	MakeBullet();
}

void ATPSPlayer::MakeBullet()
{
	FTransform t = GunComp->GetSocketTransform(TEXT("FirePoint"));
	GetWorld()->SpawnActor<ABullet>(BulletFactory, t);
}

