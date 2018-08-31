// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "PlayerSpace.h"
#include "Engine/Engine.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	//Create and poisition a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("KeyUp", IE_Pressed, this, &ABaseCharacter::KeyUp);
	InputComponent->BindAction("KeyDown", IE_Pressed, this, &ABaseCharacter::KeyDown);
	InputComponent->BindAction("KeyLeft", IE_Pressed, this, &ABaseCharacter::KeyLeft);
	InputComponent->BindAction("KeyRight", IE_Pressed, this, &ABaseCharacter::KeyRight);

	InputComponent->BindAction("Attack", IE_Pressed, this, &ABaseCharacter::Attack);
	InputComponent->BindAction("Special", IE_Pressed, this, &ABaseCharacter::Special);
}

void ABaseCharacter::SetControllerIndex(int a_iPlayerIndex, AActor* a_pWorldCamera)
{
	ThisPlayerController = UGameplayStatics::GetPlayerController(this, a_iPlayerIndex);
	if (ThisPlayerController)
	{
		ThisPlayerController->EnableInput(ThisPlayerController);
		ThisPlayerController->UnPossess();
		ThisPlayerController->Possess(this);
		ThisPlayerController->SetViewTarget(a_pWorldCamera);
	}
}

void ABaseCharacter::SetPlayerMovementController(APlayerSpace* a_pPlayerSpace)
{
	pPlayerSpace = a_pPlayerSpace;
}

void ABaseCharacter::BindPlayer(int a_iIndex)
{
	//Take control of the default player

	switch(a_iIndex)
	{
	case 0:
		AutoPossessPlayer = EAutoReceiveInput::Player0;
		break;
	case 1:
		AutoPossessPlayer = EAutoReceiveInput::Player1;
		break;
	default:
		break;
	}
}

void ABaseCharacter::SignMyselfUpForMovement()
{
	if (pPlayerSpace != nullptr)
	{
		iPlayerNumber = pPlayerSpace->AddPlayerEntity(this);
	}
}

void ABaseCharacter::KeyUp()
{
}

void ABaseCharacter::KeyDown()
{
}

void ABaseCharacter::KeyLeft()
{
	bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, false);
	if (check)
	{
		pPlayerSpace->MovePlayerHorizontal(this, false, 1);
		FVector loc = this->GetActorLocation();
		loc.X -= 100.0f;
		//SetActorLocation(loc);
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), loc, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);

		int P1 = pPlayerSpace->getP1Block();
		int P2 = pPlayerSpace->getP2Block();
		FString text = FString::FromInt(P1);
		text += " ";
		text += FString::FromInt(P2);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
	}
}

void ABaseCharacter::KeyRight()
{
	bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, true);
	if (check)
	{
		pPlayerSpace->MovePlayerHorizontal(this, true, 1);
		FVector loc = this->GetActorLocation();
		loc.X += 100.0f;
		//SetActorLocation(loc);
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), loc, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);

		int P1 = pPlayerSpace->getP1Block();
		int P2 = pPlayerSpace->getP2Block();
		FString text = FString::FromInt(P1);
		text += " ";
		text += FString::FromInt(P2);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
	}
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Special()
{
}

