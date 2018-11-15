// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldCamera.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
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
	pSpringArm = SpringArm;
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
}

// Called every frame
void AWorldCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const int32 CharacterAmount = aa_CharacterActors.Num();
	if (CharacterAmount == 2)
	{
		const FVector p1 = aa_CharacterActors[0]->GetActorLocation();
		const FVector p2 = aa_CharacterActors[1]->GetActorLocation();
		if (p1.X < p2.X)
		{
			fCharacterMiddlePoint = (p2.X + p1.X) / 2;
		}
		else
		{
			fSmallest = p1.X;
			fCharacterMiddlePoint = (p1.X + p2.X) / 2;
		}
		//fCharacterDistance = p1.X * p1.X + p2.X * p2.X;
		FVector curDis = this->GetActorLocation();
		RootComponent->SetWorldLocation(FVector(fCharacterMiddlePoint, curDis.Y, curDis.Z));
	}
	
}

