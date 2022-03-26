// Fill out your copyright notice in the Description page of Project Settings.
#include "MyCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMyCharacterPlayer::AMyCharacterPlayer()
{
	root = UObject::CreateDefaultSubobject<USceneComponent>(TEXT("root"));

	// Set size for collision capsule
	capsule = ACharacter::GetCapsuleComponent();
	capsule->SetupAttachment(root);
	capsule->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	RollTurnRate = 25.0f;
	BaseLookUpRate = 20.0f;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArms = UObject::CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArms"));
	springArms->SetupAttachment(GetCapsuleComponent());
	springArms->TargetArmLength = 0.f;
	springArms->bUsePawnControlRotation = true;
	springArms->bDoCollisionTest = false;

	// Create a CameraComponent	
	camera = UObject::CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	camera->SetupAttachment(springArms);
	camera->SetRelativeLocation(FVector(0.0f, 1.75f, 64.f)); // Position the camera
	camera->bUsePawnControlRotation = true;

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
	PlayerInputComponent->BindAxis("Rotate", this, &AMyCharacterPlayer::RotateLocalUpPlayer);

	// Look 
	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacterPlayer::CameraTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacterPlayer::LookUpAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyCharacterPlayer::LookUpAtRate);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind crouch events
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyCharacterPlayer::CrouchPlayer);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyCharacterPlayer::UnCrouchPlayer);
}

void AMyCharacterPlayer::MoveForward(float Value)
{
	if (Value == 0.0f) { return; }

	AddMovementInput(camera->GetForwardVector(), Value);
}

void AMyCharacterPlayer::MoveRight(float Value)
{
	if (Value == 0.0f) { return; }

	AddMovementInput(camera->GetRightVector(), Value);
}

void AMyCharacterPlayer::MoveUp(float Value)
{
	if (Value == 0.0f) { return; }

	APawn::AddMovementInput(camera->GetUpVector(), Value);
}

/// <summary>
/// Yaw Rotation
/// </summary>
/// <param name="Rate"></param>
void AMyCharacterPlayer::CameraTurn(float Rate)
{
	if (Rate == 0.0f) { return; }

	FRotator r = FRotator();
	r.Yaw += Rate;
	APawn::AddActorLocalRotation(r);
}

/// <summary>
/// Pitch Rotation
/// </summary>
/// <param name="Rate"></param>
void AMyCharacterPlayer::LookUpAtRate(float Rate)
{
	if (Rate == 0.0f) { return; }

	float angle = -Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();

	FRotator rotatorCamera = camera->GetRelativeTransform().GetRotation().Rotator();
	rotatorCamera.Pitch += angle;

	if (rotatorCamera.Pitch > -60 && rotatorCamera.Pitch < 60)
	{
		FRotator r = FRotator();
		r.Pitch += angle;
		camera->AddRelativeRotation(r);
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Orange, TEXT("Rotation  Loock : " + rotatorCamera.ToString()));


		//if (characterMovementComponent->IsFlying())
			//this->RotateLocalRightPlayer(angle);

	}
}

/// <summary>
/// Yaw Rotation 
/// </summary>
/// <param name="Rate"></param>
void AMyCharacterPlayer::RotateLocalRightPlayer(float Rate)
{
	/*FRotator r = FRotator();
	r.Yaw += Rate;*/
	//capsule->AddRelativeRotation(r);
}

//use pawn controlle rotation is off
//Rotate Capsule and camera
void AMyCharacterPlayer::RotateLocalUpPlayer(float Rate)
{
	if (Rate == 0.0f) { return; }
	if (characterMovementComponent->IsFlying() != true) { return; }

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Orange, TEXT("Rotation "));

	FRotator r = FRotator();
	r.Roll += Rate;
	//APawn::AddActorLocalRotation(r);
	capsule->AddLocalRotation(r);
}

void AMyCharacterPlayer::CrouchPlayer()
{
	Crouch();
}

void AMyCharacterPlayer::UnCrouchPlayer()
{
	UnCrouch();
}
