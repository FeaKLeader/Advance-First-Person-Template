// Fill out your copyright notice in the Description page of Project Settings.
#include "MyCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameSession.h"

// Sets default values
AMyCharacterPlayer::AMyCharacterPlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.0f;
	RollTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCameraComponent"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	characterMovementComponent = this->GetCharacterMovement();
}

// Called when the game starts or when spawned
void AMyCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacterPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacterPlayer::MoveRight);

	PlayerInputComponent->BindAxis("MoveUp", this, &AMyCharacterPlayer::MoveUp);

	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacterPlayer::CameraTurn);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacterPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyCharacterPlayer::LookUpAtRate);

	PlayerInputComponent->BindAxis("Rotate", this, &AMyCharacterPlayer::RotatePlayer);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind crouch events
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyCharacterPlayer::CrouchPlayer);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyCharacterPlayer::UnCrouchPlayer);
}

void AMyCharacterPlayer::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(APawn::GetActorForwardVector(), Value);
	}
}

void AMyCharacterPlayer::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(APawn::GetActorRightVector(), Value);
	}
}

void AMyCharacterPlayer::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacterPlayer::CameraTurn(float Rate)
{
	AddControllerYawInput(Rate);
}

void AMyCharacterPlayer::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacterPlayer::MoveUp(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorUpVector(), Value);
	}
}

void AMyCharacterPlayer::CrouchPlayer()
{
	Crouch();
}

void AMyCharacterPlayer::UnCrouchPlayer()
{
	UnCrouch();
}

//use pawn controlle rotation is off
//Rotate Capsule and camera
void AMyCharacterPlayer::RotatePlayer(float Rate)
{
	if (Rate != 0 && characterMovementComponent->IsFlying())
	{
		FRotator r = APawn::GetActorRotation();
		r.Roll += Rate;
		APawn::SetActorRotation(r);  
	}
}