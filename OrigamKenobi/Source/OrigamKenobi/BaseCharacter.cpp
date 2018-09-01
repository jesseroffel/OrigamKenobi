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
#include "Components/BoxComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	//HIT BOXES
	CharacterHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CharacterHitbox"));
	CharacterHitBox->SetupAttachment(RootComponent);
	CharacterHitBox->SetBoxExtent(FVector(16.0f, 32.0f, 32.0f));
	CharacterHitBox->SetHiddenInGame(false);
	CharacterHitBox->BodyInstance.SetCollisionProfileName("OverlapAll");
	CharacterHitBox->bGenerateOverlapEvents = true;
	//CharacterHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnCharacterOverlap);

	//CharacterHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OverLapFunction);

	SwordHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordHitBox"));
	SwordHitBox->SetupAttachment(RootComponent);
	SwordHitBox->SetBoxExtent(FVector(16.0f, 32.0f, 8.0f));
	SwordHitBox->SetRelativeLocation(FVector(0.0f, 20.0f, 0.0f));
	SwordHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnSwordBeginOverlap);
	SwordHitBox->bGenerateOverlapEvents = false;

	SwordHitBox->BodyInstance.SetCollisionProfileName("OverlapAll");
	//SwordHitBox->bGenerateOverlapEvents = true;
	//SetRootComponent(BladeComponent);

	//SwordHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnBeginOverlap);


	//Create and poisition a mesh component so we can see where our sphere is
	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -35.0f));
		SphereVisual->SetWorldScale3D(FVector(0.15f));
	}

	//YogaMaster
	SM_Yoga = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentationYoda"));
	SM_Yoga->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_YogaMasterAsset(TEXT("/Game/Models/SK_MasterYoga"));
	if (SM_YogaMasterAsset.Succeeded())
	{
		SM_Yoga->SetStaticMesh(SM_YogaMasterAsset.Object);
		SM_Yoga->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
		SM_Yoga->SetWorldScale3D(FVector(0.15f));
	}

	//DarkInvader
	SM_DarkInvader = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentationDarkInvader"));
	SM_DarkInvader->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DarkInvaderAsset(TEXT("/Game/Models/SK_DarkInvader"));
	if (DarkInvaderAsset.Succeeded())
	{
		SM_DarkInvader->SetStaticMesh(DarkInvaderAsset.Object);
		SM_DarkInvader->SetRelativeLocation(FVector(0.0f, 0.0f, -35.0f));
		SM_DarkInvader->SetWorldScale3D(FVector(0.15f));
		//SM_DarkInvader->bVisible = false;
	}

	PrimaryActorTick.bCanEverTick = true;
}


void ABaseCharacter::OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ABaseCharacter* OtherCharacter = Cast<ABaseCharacter>(OtherActor);
		if (OtherComp->GetName() == "CharacterHitbox" && OtherCharacter->GetAttackable())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, OverlappedComponent->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, OtherActor->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, OtherComp->GetName());
		}
	}
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
			vCharacterLocation = this->GetActorLocation();

			FString text = FString::FromInt(this->GetActorLocation().X);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
		}
	}

	if (bJumping)
	{
		if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) > fJumpTimer && bCheckJumpTime)
		{
			if (bJumpingPressed)
			{
				bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, bJumpDirection, 2);
				if (check)
				{
					bJumpMoving = true;
					bMovementLocked = true;
					bCheckJumpTime = false;

					pPlayerSpace->MovePlayerHorizontal(this, bJumpDirection, 2);
					vGoingLocation = vCharacterLocation;
					if (bJumpDirection) { vGoingLocation.X += HorizontalMovement; }
					else { vGoingLocation.X -= HorizontalMovement; }
					vGoingLocation.Z += VerticalMovement + (VerticalMovement / 4);

					FLatentActionInfo LatentInfo;
					LatentInfo.CallbackTarget = this;
					UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), vGoingLocation, this->GetActorRotation(), false, true, 0.11, false, EMoveComponentAction::Type::Move, LatentInfo);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "OCCUPIED!");
					ResetToBottom();
				}
			} 
			else
			{
				ResetToBottom();
			}
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
				vCharacterLocation = this->GetActorLocation();
				pPlayerSpace->MovePlayerVertical(this, false);

				FString text = FString::FromInt(this->GetActorLocation().Z);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
			}
		}

		if (bJumpMoving && !bJumpFinishing)
		{
			bool bNearbyX = FMath::IsNearlyEqual(this->GetActorLocation().X, vGoingLocation.X, 0.5f);
			bool bNearbyZ = FMath::IsNearlyEqual(this->GetActorLocation().Z, vGoingLocation.Z, 0.5f);
			if (bNearbyX && bNearbyZ)
			{
				bJumpFinishing = true;
				if (bJumpDirection) { vGoingLocation.X += HorizontalMovement; }
				else { vGoingLocation.X -= HorizontalMovement; }
				vGoingLocation.Z -= VerticalMovement + (VerticalMovement / 4);

				FLatentActionInfo LatentInfo;
				LatentInfo.CallbackTarget = this;
				UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), vGoingLocation, this->GetActorRotation(), true, false, 0.08, false, EMoveComponentAction::Type::Move, LatentInfo);
			}
		}
		if (bJumpMoving && bJumpFinishing)
		{
			bool bNearbyX = FMath::IsNearlyEqual(this->GetActorLocation().X, vGoingLocation.X, 0.5f);
			bool bNearbyZ = FMath::IsNearlyEqual(this->GetActorLocation().Z, vGoingLocation.Z, 0.5f);
			if (bNearbyX && bNearbyZ)
			{
				bJumping = false;
				bVerticalReset = false;
				bVerticalLocked = false;
				bHorizontalLocked = false;

				bJumpMoving = false;
				bJumpFinishing = false;
				bMovementLocked = false;

				bJumpingPressed = false;
				pPlayerSpace->MovePlayerVertical(this, false);

				vCharacterLocation = this->GetActorLocation();

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

		pPlayerSpace->MovePlayerVertical(this, true);

		vGoingLocation = vCharacterLocation;
		vGoingLocation.Z += VerticalMovement;

		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), vGoingLocation, this->GetActorRotation(), false, true, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
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

			vGoingLocation = vCharacterLocation;
			vGoingLocation.X -= HorizontalMovement;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
	}

	if (bHorizontalLocked && !bVerticalReset && !bJumpingPressed)
	{
		bJumpDirection = false;
		bJumpingPressed = true;
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
			vGoingLocation = vCharacterLocation;
			vGoingLocation.X += HorizontalMovement;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
	}

	if (bHorizontalLocked && !bVerticalReset && !bJumpingPressed)
	{
		bJumpDirection = true;
		bJumpingPressed = true;
	}
}

void ABaseCharacter::ResetToBottom()
{
	bVerticalReset = true;
	bMovementLocked = true;
	bCheckJumpTime = false;
	vGoingLocation = vCharacterLocation;
	//vGoingLocation.Z -= VerticalMovement;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(this->GetRootComponent(), vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
}

void ABaseCharacter::Attack()
{
	if (!bAttacking)
	{
		bAttacking = true;
		SwordHitBox->bGenerateOverlapEvents = true;
		SwordHitBox->SetBoxExtent(FVector(16.0f, 32.0f, 8.0f));
	}
}

void ABaseCharacter::Special()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "SPECIAL BUTTONS!");
	if (!bAttacking)
	{
		bAttacking = true;

	}
}

void ABaseCharacter::SetModelVisibleYoga()
{
	SphereVisual->ToggleVisibility();
	SphereVisual->GetAttachmentRoot()->SetVisibility(false, false);
	SM_DarkInvader->ToggleVisibility();
	SM_DarkInvader->GetAttachmentRoot()->SetVisibility(false, false);
	vCharacterLocation = this->GetActorLocation();
}

void ABaseCharacter::SetModelVisibleVader()
{
	SphereVisual->ToggleVisibility();
	SphereVisual->GetAttachmentRoot()->SetVisibility(false, false);
	SM_Yoga->ToggleVisibility();
	SM_Yoga->GetAttachmentRoot()->SetVisibility(false, false);
	vCharacterLocation = this->GetActorLocation();
}

void ABaseCharacter::SetDirection(bool a_bState)
{
	b_MirroredDirection = a_bState;

	if (!b_MirroredDirection)
		RootComponent->SetWorldRotation(FRotator(0.f, (360.0f - CharacterRotation), 0.0f));
	else
		RootComponent->SetWorldRotation(FRotator(0.f, CharacterRotation, 0.0f));
}

void ABaseCharacter::OverLapFunction()
{

}

bool ABaseCharacter::GetAttackable()
{
	return bAttackable;
}
