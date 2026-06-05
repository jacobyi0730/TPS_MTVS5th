// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComponent.h"

#include "EnhancedInputComponent.h"
#include "TPSPlayer.h"
#include "TPSPlayerAnim.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UPlayerMoveComponent::UPlayerMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerMoveComponent::SetPlayerInputComp(class UEnhancedInputComponent* input)
{
	Super::SetPlayerInputComp(input);
	
	input->BindAction(Me->IA_TPSMove, ETriggerEvent::Triggered, this, &UPlayerMoveComponent::OnMyMove);
		
	input->BindAction(Me->IA_TPSLook, ETriggerEvent::Triggered, this, &UPlayerMoveComponent::OnMyLook);
		
	input->BindAction(Me->IA_TPSJump, ETriggerEvent::Started, this, &UPlayerMoveComponent::OnMyJump);

	input->BindAction(Me->IA_TPSJog, ETriggerEvent::Started, this, &UPlayerMoveComponent::OnMyJog);
	input->BindAction(Me->IA_TPSJog, ETriggerEvent::Completed, this, &UPlayerMoveComponent::OnMyWalk);
		
	input->BindAction(Me->IA_TPSCrouch, ETriggerEvent::Started, this, &UPlayerMoveComponent::OnMyCrouch);
		
	input->BindAction(Me->IA_TPSDiveRoll, ETriggerEvent::Started, this, &UPlayerMoveComponent::OnMyDiveRoll);

}


void UPlayerMoveComponent::OnMyMove(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	FVector dir = FVector(v.X, v.Y, 0);
	dir = Me->GetControlRotation().RotateVector(dir);
	Me->AddMovementInput(dir, 1);
}

void UPlayerMoveComponent::OnMyLook(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	
	Me->AddControllerYawInput(v.X);
	Me->AddControllerPitchInput(v.Y);
}

void UPlayerMoveComponent::OnMyJump(const FInputActionValue& value)
{
	Me->Jump();
}

void UPlayerMoveComponent::OnMyJog(const struct FInputActionValue& value)
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 900.f;
}

void UPlayerMoveComponent::OnMyWalk(const struct FInputActionValue& value)
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void UPlayerMoveComponent::OnMyCrouch(const struct FInputActionValue& value)
{
	if (Me->IsCrouched())
	{
		Me->UnCrouch();
	}
	else
	{
		Me->Crouch();
	}
}

void UPlayerMoveComponent::OnMyDiveRoll(const struct FInputActionValue& value)
{
	if (auto* anim = Cast<UTPSPlayerAnim>(Me->GetMesh()->GetAnimInstance()))
	{
		anim->PlayDiveRollMontage();
	}

}