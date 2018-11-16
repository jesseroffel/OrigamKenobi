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

	SpecialHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpecialHitBox"));
	SpecialHitBox->SetupAttachment(RootComponent);
	SpecialHitBox->SetBoxExtent(FVector(42.0f, 72.0f, 8.0f));
	SpecialHitBox->SetRelativeLocation(FVector(0.0f, 20.0f, 0.0f));
	SpecialHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnSpecialBeginOverlap);
	SpecialHitBox->bGenerateOverlapEvents = false;
	SpecialHitBox->BodyInstance.SetCollisionProfileName("OverlapAll");

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
			AddSpecialPoints(10);
		} else
		{
			if (OtherComp->GetName() == "CharacterHitbox" && !OtherCharacter->GetAttackable())
			{
				bStunned = true;
				fStunTimer = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 0.75f;
				//pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::StunHitThing);
				bMovementLocked = true;
				bVerticalLocked = true;
			}
		}
	}
}

void ABaseCharacter::OnSpecialBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ABaseCharacter* OtherCharacter = Cast<ABaseCharacter>(OtherActor);
		if (OtherComp->GetName() == "CharacterHitbox" && OtherCharacter->GetAttackable())
		{
			const FString text = OtherCharacter->GetCharacterName() + " got hit by " + CharacterName;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
			bSpecialHitOther = true;
			pGameDirector->DisableControlsOther(this);
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

void ABaseCharacter::ControlledMoveDirection(bool a_bLeftDirection, int a_iAmount)
{
	if (a_bLeftDirection)
	{
		const bool checkLeft = pPlayerSpace->CheckMovePlayerHorizontal(this, false);
		if (checkLeft)
		{
			pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::Jump);

			pPlayerSpace->MovePlayerHorizontal(this, false, 1);
			vCharacterLocation = this->GetActorLocation();
			vGoingLocation = vCharacterLocation;
			vGoingLocation.X -= HorizontalMovement;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
	}
	else
	{
		const bool checkRight = pPlayerSpace->CheckMovePlayerHorizontal(this, true);
		if (checkRight)
		{
			pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::Jump);

			pPlayerSpace->MovePlayerHorizontal(this, true, 1);
			vCharacterLocation = this->GetActorLocation();
			vGoingLocation = vCharacterLocation;
			vGoingLocation.X += HorizontalMovement;

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
	}
	
}

void ABaseCharacter::DisableTheInput()
{
	bMovementLocked = true;
	bVerticalLocked = true;
	bHorizontalLocked = true;
	bCanFall = false;

	//Move character up
	vCharacterLocation = this->GetActorLocation();
	pPlayerSpace->MovePlayerVertical(this, true);
	vGoingLocation = vCharacterLocation;
	vGoingLocation.Z += VerticalMovement;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, true, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
}

void ABaseCharacter::EnableTheInput()
{
	bMovementLocked = false;
	bVerticalLocked = false;
	bHorizontalLocked = false;
	bCanFall = true;

	//Move character down
	vCharacterLocation = this->GetActorLocation();
	pPlayerSpace->MovePlayerVertical(this, false);
	vGoingLocation = vCharacterLocation;
	vGoingLocation.Z -= VerticalMovement;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, true, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	fSpecialAddWait = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 5.0f;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameDirector::StaticClass(), FoundActors);
	AGameDirector* Director = dynamic_cast<AGameDirector*>(FoundActors[0]);
	pGameDirector = Director;
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

	//Special Bar
	if (fWorldTime > fSpecialAddWait)
	{
		AddSpecialPoints(1);
		fSpecialAddWait = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 1.0f;
	}
	//Special Checks
	if (bSpecialPressed)
	{
		if (!bMovementLocked && !bVerticalLocked && !bHorizontalLocked && !bAttacking && !bSpecialActivated)
		{
			if (iSpecial == 100)
			{
				ActivateSpecial();
			}
		}
	}
	//If Activated, check for hit, if so, control
	if (bSpecialActivated)
	{
		//if check for timer
		if (fWorldTime < fSpecialHitPerson && bCheckSpecialTimer)
		{
			if (bSpecialHitOther)
			{
				const FString text = CharacterName + " Control Hit Other";
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
				DisableSpecial();
				bCheckSpecialTimer = false;
				bMovementLocked = true;
				bVerticalLocked = true;
				bHorizontalLocked = true;
				fSpecialPunishTimer = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 5.0f;
			}
		}
		if (fWorldTime > fSpecialHitPerson && bCheckSpecialTimer)
		{	//Timer passed, no one hit, get locked
			const FString text = CharacterName + " Control Stun Activate";
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
			bStunned = true;
			fStunTimer = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 2.5f;
			bMovementLocked = true;
			bVerticalLocked = true;
			bHorizontalLocked = true;
			bSpecialActivated = false;
			bCheckSpecialTimer = false;
			DisableSpecial();
		}
		if (bSpecialHitOther && !bCheckSpecialTimer)
		{
			//Control other until fSpecialPunishTimer is passed
			if (fWorldTime < fSpecialPunishTimer)
			{
				if (bDirectionPressed)
				{
					if (bRightDirectionPressed) { ControlOtherRight(); }
					else { ControlOtherLeft(); }
				}
			}
			else
			{
				//Disable special after controlling
				pGameDirector->EnableControlsOther(this);
				bSpecialActivated = false;
				bMovementLocked = false;
				bVerticalLocked = false;
				bHorizontalLocked = false;
				bSpecialHitOther = false;
				bCheckSpecialTimer = false;
				const FString text = CharacterName + " Control time over";
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
			}
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
	bSpecialPressed = false;
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
	FVector location = pGameDirector->RespawnThisPlayer(this);
	pPlayerSpace->ResetPlacement(this, location);
	if (pPlayerSpace->CheckIfOccupied(this, location))
	{
		location = pPlayerSpace->GetOtherSpawn(location);
		pPlayerSpace->CheckWhoIsLeftRight();
	}

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(RootComponent, location, this->GetActorRotation(), false, false, 0.0f, true, EMoveComponentAction::Type::Move, LatentInfo);

	if (iLives > 0)
	{
		iLives--;
		iHealth = 100;
	}
	FString text = CharacterName + " Lives: " + FString::FromInt(iLives);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
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
	bSpecialPressed = true;
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
		const bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, false, 1);
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


			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
		bCheckMoveTimer = false;
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
		const bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, true);
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


			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(RootComponent, vGoingLocation, this->GetActorRotation(), false, false, 0.10, false, EMoveComponentAction::Type::Move, LatentInfo);
		}
		bCheckMoveTimer = false;
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
	}
	if (bSelfHit)
	{
		pPlayerSpace->PlayCharacterAnimation(this, EAnimationType::SelfStab);
		iHealth -= 10;
		FString text = CharacterName + " HP: " + FString::FromInt(iHealth);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
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
	iHealth -= 5;
	FString text = CharacterName + " HP: " + FString::FromInt(iHealth);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
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
	bHorizontalLocked = false;
	vCharacterLocation = this->GetActorLocation();
}

void ABaseCharacter::AddSpecialPoints(int a_iAmount)
{
	if (iSpecial < iSpecialMax)
	{
		iSpecial += a_iAmount;
		if (iSpecial > iSpecialMax) { iSpecial = iSpecialMax;}
	}
}

void ABaseCharacter::ActivateSpecial()
{
	bSpecialActivated = true;
	iSpecial = 0;
	fSpecialAddWait = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 10.0f;
	fSpecialHitPerson = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + 0.25f;
	bCheckSpecialTimer = true;

	SpecialHitBox->bGenerateOverlapEvents = true;
	//SpecialHitBox->SetHiddenInGame(false);
	SpecialHitBox->SetBoxExtent(FVector(42.05f, 72.05f, 8.05f));

	bMovementLocked = true;
	bHorizontalLocked = true;
	bVerticalLocked = true;
}

void ABaseCharacter::DisableSpecial()
{
	SpecialHitBox->bGenerateOverlapEvents = false;
	//SpecialHitBox->SetHiddenInGame(true);
	SpecialHitBox->SetBoxExtent(FVector(42.00f, 72.00f, 8.00f));
}

void ABaseCharacter::ControlOtherLeft()
{
	const FString text = CharacterName + " controlled to the left";
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);

	pGameDirector->SpecialControlOtherDirection(this, true);
}

void ABaseCharacter::ControlOtherRight()
{
	const FString text = CharacterName + " controlled to the right";
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
	pGameDirector->SpecialControlOtherDirection(this, false);
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
				const bool check = pPlayerSpace->CheckMovePlayerHorizontal(this, bJumpDirection, 2);
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
