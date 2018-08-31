// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldCamera.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Engine.h"

// Sets default values
AWorldCamera::AWorldCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SphereComponent as root
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;

	//Create Springarm
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	//SpringArm->RelativeRotation = FRotator(0.f, -10.0f, 90.f);
	SpringArm->TargetArmLength = 100.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	//Setup Camera to arm
	UCameraComponent* StageCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("StageCamera"));
	StageCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AWorldCamera::BeginPlay()
{
	Super::BeginPlay();
	//if (aa_CharacterActors.Num() <= 1) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ADD All CharacterActors!!!")); }
}

// Called every frame
void AWorldCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

