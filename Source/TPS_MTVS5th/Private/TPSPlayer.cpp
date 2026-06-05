// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "Bullet.h"
#include "Enemy.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PlayerFireComponent.h"
#include "PlayerMoveComponent.h"
#include "TPSPlayerAnim.h"
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
	GunComp->SetupAttachment(GetMesh(), FName("hand_r"));
	GunComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SniperComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("SniperComp"));
	SniperComp->SetupAttachment(GetMesh(), FName("hand_r"));
	SniperComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}
	
	
	// 이동, 공격 컴포넌트를 생성하고싶다.
	MoveComp = CreateDefaultSubobject<UPlayerMoveComponent>(FName("MoveComp"));
	FireComp = CreateDefaultSubobject<UPlayerFireComponent>(FName("FireComp"));
	
	
	
	// 회전정보를 설정하고싶다.
	bUseControllerRotationYaw = true;
	CameraBoomComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false; 
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATPSPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
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
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	
	
	if (auto* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		MoveComp->SetPlayerInputComp(input);
		FireComp->SetPlayerInputComp(input);
	}
}


