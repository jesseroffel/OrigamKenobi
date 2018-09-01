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
	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.25f));
	}

	//YogaMaster
	SM_Yoga = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentationYoda"));
	SM_Yoga->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_YogaMasterAsset(TEXT("/Game/Models/SM_Yoda"));
	if (SM_YogaMasterAsset.Succeeded())
	{
		SM_Yoga->SetStaticMesh(SM_YogaMasterAsset.Object);
		SM_Yoga->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SM_Yoga->SetWorldScale3D(FVector(0.25f));
		//SM_Yoga->bVisible = false;
	}

	//DarthInvader
	SM_DarthInvader = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentationDarthInvader"));
	SM_DarthInvader->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_DarthInvaderAsset(TEXT("/Game/Models/SM_DarthVader"));
	if (SM_DarthInvaderAsset.Succeeded())
	{
		SM_DarthInvader->SetStaticMesh(SM_DarthInvaderAsset.Object);
		SM_DarthInvader->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SM_DarthInvader->SetWorldScale3D(FVector(0.25f));
		//SM_DarthInvader->bVisible = false;
	}

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
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

	switch (a_iIndex)
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

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoving)
	{
		bool bNearby = FMath::IsNearlyEqual(this->GetActorLocation().X, vGoingLocation.X, 0.5f);
		if (bNearby)
		{
			bMoving = false;
			bMovementLocked = false;
			bVerticalLocked = false;

			FString text = FString::FromInt(this->GetActorLocation().X);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
		}
	}

	if (bJumping)
	{
		if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) > fJumpTimer && bCheckJumpTime)
		{
			bVerticalReset = true;
			bMovementLocked = true;
			bCheckJumpTime = false;
			ResetToBottom();
		}

		if (bVerticalReset)
		{
			bool bNearby = FMath::IsNearlyEqual(this->GetActorLocation().Z, vGoingLocation.Z, 0.5f);
			if (bNearby)
			{
				bJumping = false;
				bVerticalReset = false;
				bMovementLocked = false;
				bVerticalLocked = false;
				bHorizontalLocked = false;

				FString text = FString::FromInt(this->GetActorLocation().Z);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
			}
		}

		if (bJumpMoving)
		{
			bool bNearby = FMath::IsNearlyEqual(this->GetActorLocation().X, vGoingLocation.X, 0.5f);
			if (bNearby)
			{
				bJumping = false;
				bVerticalReset = false;
				bVerticalLocked = false;
				bHorizontalLocked = false;

				bJumpMoving = false;
				bMovementLocked = false;

				FString text = FString::FromInt(this->GetActorLocation().Z);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
			}
		}
	}
}



void ABaseCharacter::KeyUp()
{
	if (!bJumping && !bVerticalLocked)
	{
		bJumping = true;
		bVerticalLocked = true;
		bCheckJumpTime = true;
		bHorizontalLocked = true;
		fJumpTimer = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 0.20f;

		vGoingLocation = this->GetActorLocation();
		vGoingLocation.Z += VerticalMovement;

		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
	}
}

void ABaseCharacter::KeyDown()
{
}

void ABaseCharacter::KeyLeft()
{
	if (!bMovementLocked && !bHorizontalLocked)
	{
		bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, false, 1);
		if (check)
		{
			bMoving = true;
			bMovementLocked = true;
			bVerticalLocked = true;
			pPlayerSpace->MovePlayerHorizontal(this, false, 1);
			vGoingLocation = this->GetActorLocation();
			vGoingLocation.X -= HorizontalMovement;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
	}
}

void ABaseCharacter::KeyRight()
{
	if (!bMovementLocked && !bHorizontalLocked)
	{
		bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, true);
		if (check)
		{
			bMoving = true;
			bMovementLocked = true;
			bVerticalLocked = true;
			pPlayerSpace->MovePlayerHorizontal(this, true, 1);
			vGoingLocation = this->GetActorLocation();
			vGoingLocation.X += HorizontalMovement;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
	}

	if (bHorizontalLocked && !bVerticalReset)
	{
		bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, true, 2);
		if (check) 
		{
			bJumpMoving = true;
			bMovementLocked = true;

			pPlayerSpace->MovePlayerHorizontal(this, true, 2);
			vGoingLocation = this->GetActorLocation();
			vGoingLocation.X += HorizontalMovement * 2;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "OCCUPIED!");
		}
	}
}

void ABaseCharacter::ResetToBottom()
{
	// push down, check for position and then reset movement
	vGoingLocation = this->GetActorLocation();
	vGoingLocation.Z -= VerticalMovement;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Special()
{
}

void ABaseCharacter::SetModelVisibleYoga()
{
	SphereVisual->ToggleVisibility();
	SphereVisual->GetAttachmentRoot()->SetVisibility(false, false);
	SM_DarthInvader->ToggleVisibility();
	SM_DarthInvader->GetAttachmentRoot()->SetVisibility(false, false);
}

void ABaseCharacter::SetModelVisibleVader()
{
	SphereVisual->ToggleVisibility();
	SphereVisual->GetAttachmentRoot()->SetVisibility(false, false);
	SM_Yoga->ToggleVisibility();
	SM_Yoga->GetAttachmentRoot()->SetVisibility(false, false);
}

void ABaseCharacter::SetDirection(bool a_bState)
{
	b_MirroredDirection = true;
}

