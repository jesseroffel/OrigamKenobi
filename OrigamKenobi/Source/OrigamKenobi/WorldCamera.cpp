// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldCamera.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStaticsTypes.h"

// Sets default values
AWorldCamera::AWorldCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SphereComponent as root
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Create Springarm
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	pSpringArm = SpringArm;
	SpringArm->SetupAttachment(RootComponent);
	//SpringArm->RelativeRotation = FRotator(0.f, -10.0f, 90.f);
	SpringArm->TargetArmLength = 125.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bIsCameraFixed = false;

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
		fCharacterDistance = 100.0f;
		if (p1.X < p2.X)
		{
			fCharacterMiddlePoint = (p2.X + p1.X) / 2;
			fCharacterDistance += (p2.X - p1.X) * 0.5f;
		}
		else
		{
			fCharacterMiddlePoint = (p1.X + p2.X) / 2;
			fCharacterDistance += (p1.X - p2.X) * 0.5f;
		}
		if (fCharacterDistance < DistanceXMin) { fCharacterDistance = DistanceXMin;}
		if (fCharacterDistance > DistanceXMax) { fCharacterDistance = DistanceXMax;}
		FVector vGoingLocation = this->GetActorLocation();
		vGoingLocation.X = fCharacterMiddlePoint;
		vGoingLocation.Y = fCharacterDistance;
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, false, 0.5, false, EMoveComponentAction::Type::Move, LatentInfo);
		//RootComponent->SetWorldLocation(FVector(fCharacterMiddlePoint, fCharacterDistance, curDis.Z));

	}
	
}

