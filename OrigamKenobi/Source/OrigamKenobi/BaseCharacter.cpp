// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "PlayerSpace.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameDirector.h"


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

	//YogaMaster static mesh
	SK_YogaMaster = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VisualRepresentationYoda"));
	SK_YogaMaster->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_YogaMasterAsset(TEXT("/Game/Models/SK_MasterYoga"));
	if (SM_YogaMasterAsset.Succeeded())
	{
		SK_YogaMaster->SetSkeletalMesh(SM_YogaMasterAsset.Object);
		SK_YogaMaster->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
		SK_YogaMaster->SetWorldScale3D(FVector(0.15f));
	}

	//DarkInvader Skeletal mesh
	SK_DarkInvader = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VisualRepresentationDarkInvader"));
	SK_DarkInvader->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DarkInvaderAsset(TEXT("/Game/Models/SK_DarkInvader"));
	if (DarkInvaderAsset.Succeeded())
	{
		SK_DarkInvader->SetSkeletalMesh(DarkInvaderAsset.Object);
		SK_DarkInvader->SetRelativeLocation(FVector(0.0f, 0.0f, -35.0f));
		SK_DarkInvader->SetWorldScale3D(FVector(0.15f));
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
			if (pPlayerSpace->IsHitDirectionLeft(this))
			{ OtherCharacter->AttackHitMe(true); }
			else { OtherCharacter->AttackHitMe(false); }
			bSuccessfulHit = true;
		}
	}
}

void ABaseCharacter::SetFallingState()
{
	bFalling = true;
	bMovementLocked = true;
	bVerticalLocked = true;
	bAttackable = false;
	pPlayerSpace->MovePlayerVertical(this, false);
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
	const float fWorldTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	//Check key combinations

	if (bCheckMoveTimer)
	{
		if (fWorldTime > fBeforeMovingTimer)
		{
			if (bRightDirectionPressed) { MoveRight(); }
			else { MoveLeft(); }
		}
		if (fWorldTime < fBeforeMovingTimer && bActionButtonPressed)
		{
			bCombinationTriggered = true;
			bMoving = false;
			bCheckMoveTimer = false;
			SideAttack();
		}
	}

	if (bActionButtonPressed && !bDirectionPressed) { BlockAttack(); }
	if (bFalling)
	{
		if(pPlayerSpace->CheckForKillPosition(this))
		{
			Respawn();
		} else
		{
			vCharacterLocation = this->GetActorLocation();
			vGoingLocation = vCharacterLocation;
			vGoingLocation.Z -= VerticalMovement;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, true, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
	}
	CheckPlayerMove();
	CheckPlayerJump();
	CheckPlayerAttacking(fWorldTime);
	CheckPlayerBlocking(fWorldTime);
	

	if (bStunned)
	{
		if (fWorldTime > fStunTimer) { RemoveStun(); }
	}

	//Reset buttons
	bActionButtonPressed = false;
	bDirectionPressed = false;
	bCombinationTriggered = false;
}



void ABaseCharacter::KeyUp()
{
	if (!bJumping && !bVerticalLocked)
	{
		pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::Jump);
		bJumping = true;
		bVerticalLocked = true;
		bCheckJumpTimer = true;
		bHorizontalLocked = true;
		fJumpTimer = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 0.20f;
		vCharacterLocation = this->GetActorLocation();
		pPlayerSpace->MovePlayerVertical(this, true);

		vGoingLocation = vCharacterLocation;
		vGoingLocation.Z += VerticalMovement;

		bAttackable = false;

		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, true, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
	}
}

void ABaseCharacter::KeyLeft()
{
	bDirectionPressed = true;
	bRightDirectionPressed = false;
	if (!bMovementLocked)
	{
		bCheckMoveTimer = true;
		fBeforeMovingTimer = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 0.045f;
	}
}

void ABaseCharacter::KeyRight()
{
	bDirectionPressed = true;
	bRightDirectionPressed = true;
	if (!bMovementLocked)
	{
		bCheckMoveTimer = true;
		fBeforeMovingTimer = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 0.045f;
	}
}

void ABaseCharacter::ResetToBottom()
{
	pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::Idle);
	bVerticalReset = true;
	bMovementLocked = true;
	bCheckJumpTimer = false;
	bJumpingPressed = false;
	vGoingLocation = vCharacterLocation;
	//vGoingLocation.Z -= VerticalMovement;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
}


void ABaseCharacter::Respawn()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameDirector::StaticClass(), FoundActors);
	AGameDirector* Director = dynamic_cast<AGameDirector*>(FoundActors[0]);

	FVector location = Director->RespawnThisPlayer(this);
	pPlayerSpace->ResetPlacement(this, location);
	if (pPlayerSpace->CheckIfOccupied(this, location))
	{
		location = pPlayerSpace->GetOtherSpawn(location);
		pPlayerSpace->CheckWhoIsLeftRight();
	}

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(RootComponent, location, this->GetActorRotation(), false, false, 0.0f, true, EMoveComponentAction::Type::Move, LatentInfo);

	bFalling = false;
	bMovementLocked = false;
	bVerticalLocked = false;
	bAttackable = true;
	pPlayerSpace->MovePlayerVertical(this, true);
}

void ABaseCharacter::Attack()
{
	bActionButtonPressed = true;
}

void ABaseCharacter::Special()
{

}

void ABaseCharacter::SetModelVisibleYoga()
{
	SK_YogaMaster->ToggleVisibility();
	SK_YogaMaster->GetAttachmentRoot()->SetVisibility(false, false);
	CharacterName = "Dark Invader";
	CharacterType = DarkInvader;
	vCharacterLocation = this->GetActorLocation();
}

void ABaseCharacter::SetModelVisibleVader()
{
	SK_DarkInvader->ToggleVisibility();
	SK_DarkInvader->GetAttachmentRoot()->SetVisibility(false, false);
	CharacterName = "Master Yoga";
	CharacterType = YogaMaster;
	vCharacterLocation = this->GetActorLocation();
}

void ABaseCharacter::SetDirection(bool a_bState)
{
	b_MirroredDirection = a_bState;

	if (!b_MirroredDirection)
		RootComponent->SetWorldRotation(FRotator(0.f, 360.0f - CharacterRotation, 0.0f));
	else
		RootComponent->SetWorldRotation(FRotator(0.f, CharacterRotation, 0.0f));
}


bool ABaseCharacter::GetAttackable() const
{
	return bAttackable;
}

void ABaseCharacter::MoveLeft()
{
	if (!bMovementLocked && !bHorizontalLocked)
	{
		bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, false, 1);
		if (check)
		{
			//use HitMyself function to check direction... lazy...
			const bool bBackWards = pPlayerSpace->HitMySelf(this, false);
			if (bBackWards) { pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::MoveBackward); }
			else { pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::MoveForward);}
			bMoving = true;
			bMovementLocked = true;
			bVerticalLocked = true;
			bJumpingPressed = false;

			pPlayerSpace->MovePlayerHorizontal(this, false, 1);
			vCharacterLocation = this->GetActorLocation();
			vGoingLocation = vCharacterLocation;
			vGoingLocation.X -= HorizontalMovement;

			bCheckMoveTimer = false;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
	}

	if (!bMovementLocked && bHorizontalLocked && !bVerticalReset && !bJumpingPressed)
	{
		bJumpDirection = false;
		bJumpingPressed = true;
	}
}

void ABaseCharacter::MoveRight()
{
	if (!bMovementLocked && !bHorizontalLocked)
	{
		bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, true);
		if (check)
		{
			//use HitMyself function to check direction... lazy...
			const bool bBackWards = pPlayerSpace->HitMySelf(this, bRightDirectionPressed);
			if (bBackWards) { pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::MoveBackward); }
			else { pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::MoveForward);}
			bMoving = true;
			bMovementLocked = true;
			bVerticalLocked = true;
			bJumpingPressed = false;
			pPlayerSpace->MovePlayerHorizontal(this, true, 1);
			vCharacterLocation = this->GetActorLocation();
			vGoingLocation = vCharacterLocation;
			vGoingLocation.X += HorizontalMovement;

			bCheckMoveTimer = false;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
	}

	if (!bMovementLocked && bHorizontalLocked && !bVerticalReset && !bJumpingPressed)
	{
		bJumpDirection = true;
		bJumpingPressed = true;
	}
}


void ABaseCharacter::SideAttack()
{
	bSelfHit = pPlayerSpace->HitMySelf(this, bRightDirectionPressed);
	if (!bAttacking && !bHorizontalLocked && !bVerticalLocked && !bSelfHit)
	{
		pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::Attack);
		bAttacking = true;
		bMovementLocked = true;
		bVerticalLocked = true;

		bCheckAttackTimer = true;
		bCheckOverlapAttack = true;
		fCheckOverlapAttackTimer = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 0.15f;
		fAttackingTimer = fCheckOverlapAttackTimer + 0.60f;

		SwordHitBox->bGenerateOverlapEvents = true;
		SwordHitBox->SetBoxExtent(FVector(16.0f, 32.15f, 8.0f));
		//SwordHitBox->SetHiddenInGame(false);
	}
	if (bSelfHit)
	{
		pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::SelfStab);
		bStunned = true;
		fStunTimer = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 0.60f;
		bMovementLocked = true;
		bVerticalLocked = true;
	}
}

void ABaseCharacter::BlockAttack()
{
	if (!bBlocking && !bHorizontalLocked && !bVerticalLocked)
	{
		pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::Block);
		bBlocking = true;
		bAttackable = false;
		bHorizontalLocked = true;
		bMovementLocked = true;
		bVerticalLocked = true;

		bCheckBlockTimer = true;
		fBlockTimer = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 0.25f;

		//CharacterHitBox->SetHiddenInGame(false);
	}
}

void ABaseCharacter::AttackHitMe(bool a_bLeftDirection)
{
	// Direction = where hit character should go
	pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::TakeDamage);
	bStunned = true;
	fStunTimer = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 0.60f;
	bMovementLocked = true;
	bVerticalLocked = true;
	if (a_bLeftDirection)
	{
		pPlayerSpace->MovePlayerHorizontal(this, false, 1);
		//FString pos1 = "P1: " + FString::FromInt(pPlayerSpace->getP1Block());
		//pos1 += " P2: " + FString::FromInt(pPlayerSpace->getP2Block());
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, pos1);
		vGoingLocation = vCharacterLocation;
		vGoingLocation.X -= HorizontalMovement;
	}
	else
	{
		pPlayerSpace->MovePlayerHorizontal(this, true, 1);
		//FString pos1 = "P1: " + FString::FromInt(pPlayerSpace->getP1Block());
		//pos1 += " P2: " + FString::FromInt(pPlayerSpace->getP2Block());
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, pos1);
		vGoingLocation = vCharacterLocation;
		vGoingLocation.X += HorizontalMovement;
	}

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
}

void ABaseCharacter::RemoveStun()
{
	bStunned = false;
	bMoving = false;
	bMovementLocked = false;
	bVerticalLocked = false;
	vCharacterLocation = this->GetActorLocation();
}

void ABaseCharacter::CheckPlayerMove()
{
	if (bMoving)
	{
		bool bNearby = FMath::IsNearlyEqual(this->GetActorLocation().X, vGoingLocation.X, 0.5f);
		if (bNearby)
		{
			bMoving = false;
			bMovementLocked = false;
			bVerticalLocked = false;
			vCharacterLocation = this->GetActorLocation();
		}
	}
}

void ABaseCharacter::CheckPlayerJump()
{
	if (bJumping)
	{
		if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) > fJumpTimer && bCheckJumpTimer)
		{
			if (bJumpingPressed)
			{
				bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, bJumpDirection, 2);
				if (check)
				{
					//use HitMyself function to check direction... lazy...
					const bool bBackWards = pPlayerSpace->HitMySelf(this, bRightDirectionPressed);
					if (bBackWards) { pPlayerSpace->PlayCharacterAnimation(this, JumpBackWard); }
					else { pPlayerSpace->PlayCharacterAnimation(this, JumpForward); }
					bJumpMoving = true;
					bMovementLocked = true;
					bCheckJumpTimer = false;

					pPlayerSpace->MovePlayerHorizontal(this, bJumpDirection, 2);

					vGoingLocation = vCharacterLocation;
					if (bJumpDirection) { vGoingLocation.X += HorizontalMovement; }
					else { vGoingLocation.X -= HorizontalMovement; }
					vGoingLocation.Z += VerticalMovement + (VerticalMovement / 4);

					FLatentActionInfo LatentInfo;
					LatentInfo.CallbackTarget = this;
					UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, true, 0.11, false, EMoveComponentAction::Type::Move, LatentInfo);
				}
				else { ResetToBottom(); }
			}
			else { ResetToBottom(); }
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
				bJumpingPressed = false;
				bCheckMoveTimer = false;

				bAttackable = true;

				vCharacterLocation = this->GetActorLocation();
				pPlayerSpace->MovePlayerVertical(this, false);
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
				UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), true, false, 0.08, false, EMoveComponentAction::Type::Move, LatentInfo);
			}
		}
		if (bJumpMoving && bJumpFinishing)
		{
			bool bNearbyX = FMath::IsNearlyEqual(this->GetActorLocation().X, vGoingLocation.X, 0.5f);
			bool bNearbyZ = FMath::IsNearlyEqual(this->GetActorLocation().Z, vGoingLocation.Z, 0.5f);
			if (bNearbyX && bNearbyZ)
			{
				bJumping = false;
				bAttackable = true;
				bVerticalReset = false;
				bVerticalLocked = false;
				bHorizontalLocked = false;

				bJumpMoving = false;
				bJumpFinishing = false;
				bMovementLocked = false;

				bJumpingPressed = false;
				bCheckMoveTimer = false;
				bCheckJumpTimer = false;

				pPlayerSpace->MovePlayerVertical(this, false);

				vCharacterLocation = this->GetActorLocation();
			}
		}
	}
}

void ABaseCharacter::CheckPlayerAttacking(float const fWorldTime)
{
	if (bAttacking)
	{
		if (bCheckOverlapAttack)
		{
			if (fWorldTime <= fCheckOverlapAttackTimer)
			{
				if (bSuccessfulHit)
				{
					bSuccessfulHit = false;
					bCheckOverlapAttack = false;

					SwordHitBox->bGenerateOverlapEvents = false;
					SwordHitBox->SetBoxExtent(FVector(16.0f, 32.0f, 8.0f));
				}
			} else
			{
				bCheckOverlapAttack = false;
				SwordHitBox->bGenerateOverlapEvents = false;
			}
		} else
		{
			if (bCheckAttackTimer && fWorldTime > fAttackingTimer)
			{
				bAttacking = false;
				bMovementLocked = false;
				bVerticalLocked = false;
				bCheckAttackTimer = false;
				SwordHitBox->SetBoxExtent(FVector(16.0f, 32.0f, 8.0f));
				//SwordHitBox->SetHiddenInGame(true);
			}
		}
	}
}

void ABaseCharacter::CheckPlayerBlocking(float const fWorldTime)
{
	if (bBlocking)
	{
		if (bCheckBlockTimer && fWorldTime > fBlockTimer)
		{
			pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::Idle);
			bBlocking = false;
			bAttackable = true;
			bMovementLocked = false;
			bHorizontalLocked = false;
			bVerticalLocked = false;
			bCheckBlockTimer = false;
			bCheckMoveTimer = false;
			//CharacterHitBox->SetHiddenInGame(true);
		}
	}
}
