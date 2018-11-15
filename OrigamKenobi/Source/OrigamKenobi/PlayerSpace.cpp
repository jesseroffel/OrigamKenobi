// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSpace.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameDirector.h"


// Sets default values
APlayerSpace::APlayerSpace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	iP1BlockUnit = iP1StartX;
	iP2BlockUnit = iP2StartX;

	//Dark invader animations
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimIdleDark(TEXT("/Game/Animations/Anim_DarkInvader_Idle"));
	if (AU_AnimIdleDark.Succeeded()) { a_IdleDark = AU_AnimIdleDark.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimAttackDark(TEXT("/Game/Animations/Anim_DarkInvader_Attack"));
	if (AU_AnimAttackDark.Succeeded()) { a_AttackDark = AU_AnimAttackDark.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimBlockDark(TEXT("/Game/Animations/Anim_DarkInvader_Block"));
	if (AU_AnimBlockDark.Succeeded()) { a_BlockDark = AU_AnimBlockDark.Object; }


	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimMoveForwardDark(TEXT("/Game/Animations/Anim_DarkInvader_MoveForward"));
	if (AU_AnimMoveForwardDark.Succeeded()) { a_MoveForwardDark = AU_AnimMoveForwardDark.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimMoveBackwardDark(TEXT("/Game/Animations/Anim_DarkInvader_MoveBackward"));
	if (AU_AnimMoveBackwardDark.Succeeded()) { a_MoveBackwardDark = AU_AnimMoveBackwardDark.Object; }

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimJumpDark(TEXT("/Game/Animations/Anim_DarkInvader_Jump"));
	if (AU_AnimJumpDark.Succeeded()) { a_JumpDark = AU_AnimJumpDark.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimJumpForwardDark(TEXT("/Game/Animations/Anim_DarkInvader_JumpForward"));
	if (AU_AnimJumpForwardDark.Succeeded()) { a_JumpForwardDark = AU_AnimJumpForwardDark.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimJumpBackwardDark(TEXT("/Game/Animations/Anim_DarkInvader_JumpBackward"));
	if (AU_AnimJumpBackwardDark.Succeeded()) { a_JumpBackwardDark = AU_AnimJumpBackwardDark.Object; }

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimSelfStabDark(TEXT("/Game/Animations/Anim_DarkInvader_SelfStab"));
	if (AU_AnimSelfStabDark.Succeeded()) { a_SelfStabDark = AU_AnimSelfStabDark.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimTakeDamageDark(TEXT("/Game/Animations/Anim_DarkInvader_TakeDamage"));
	if (AU_AnimTakeDamageDark.Succeeded()) { a_TakeDamageDark = AU_AnimTakeDamageDark.Object; }

	//Master Yoga animations
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimIdleYoga(TEXT("/Game/Animations/Anim_MasterYoga_Idle"));
	if (AU_AnimIdleYoga.Succeeded()) { a_IdleYoga = AU_AnimIdleYoga.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimAttackYoga(TEXT("/Game/Animations/Anim_MasterYoga_Attack"));
	if (AU_AnimAttackYoga.Succeeded()) { a_AttackYoga = AU_AnimAttackYoga.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimBlockYoga(TEXT("/Game/Animations/Anim_MasterYoga_Block"));
	if (AU_AnimBlockYoga.Succeeded()) { a_BlockYoga = AU_AnimBlockYoga.Object; }


	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimMoveForwardYoga(TEXT("/Game/Animations/Anim_MasterYoga_MoveForward"));
	if (AU_AnimMoveForwardYoga.Succeeded()) { a_MoveForwardYoga = AU_AnimMoveForwardYoga.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimMoveBackwardYoga(TEXT("/Game/Animations/Anim_MasterYoga_MoveBackward"));
	if (AU_AnimMoveBackwardYoga.Succeeded()) { a_MoveBackwardYoga = AU_AnimMoveBackwardYoga.Object; }

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimJumpYoga(TEXT("/Game/Animations/Anim_MasterYoga_Jump"));
	if (AU_AnimJumpYoga.Succeeded()) { a_JumpYoga = AU_AnimJumpYoga.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimJumpForwardYoga(TEXT("/Game/Animations/Anim_MasterYoga_JumpForward"));
	if (AU_AnimJumpForwardYoga.Succeeded()) { a_JumpForwardYoga = AU_AnimJumpForwardYoga.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimJumpBackwardYoga(TEXT("/Game/Animations/Anim_MasterYoga_JumpBackward"));
	if (AU_AnimJumpBackwardYoga.Succeeded()) { a_JumpBackwardYoga = AU_AnimJumpBackwardYoga.Object; }

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimSelfStabYoga(TEXT("/Game/Animations/Anim_MasterYoga_SelfStab"));
	if (AU_AnimSelfStabYoga.Succeeded()) { a_SelfStabYoga = AU_AnimSelfStabYoga.Object; }
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AU_AnimTakeDamageYoga(TEXT("/Game/Animations/Anim_MasterYoga_TakeDamage"));
	if (AU_AnimTakeDamageYoga.Succeeded()) { a_TakeDamageYoga = AU_AnimTakeDamageYoga.Object; }
}

// Called when the game starts or when spawned
void APlayerSpace::BeginPlay()
{
	Super::BeginPlay();
	bPlayerOneLeft = iP1BlockUnit < iP2BlockUnit;
	if (bPlayerOneLeft)
	{
		if (PlayerOne) { PlayerOne->SetDirection(false); }
		if (PlayerTwo) { PlayerTwo->SetDirection(true); }
	}
	else
	{
		if (PlayerOne) { PlayerOne->SetDirection(true); }
		if (PlayerTwo) { PlayerTwo->SetDirection(false); }
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(AActor::GetWorld(), AGameDirector::StaticClass(), FoundActors);
	AGameDirector* GameDirector = dynamic_cast<AGameDirector*>(FoundActors[0]);
	aa_pPlayer1Spawn = GameDirector->aa_pPlayer1Spawn;
	aa_pPlayer2Spawn = GameDirector->aa_pPlayer2Spawn;
}

bool APlayerSpace::CheckMovement(int left, int right, int amount)
{
	if (left < right) { return (left += amount) != right; }
	return (left += amount) != right;
}

bool APlayerSpace::HitMySelf(ABaseCharacter* a_Player, bool a_RightDirectionPressed)
{
	if (!a_Player) { return false; }
	if (a_Player == PlayerOne)
	{
		if (a_RightDirectionPressed) { return iP1BlockUnit > iP2BlockUnit; }
		return iP1BlockUnit < iP2BlockUnit;
	}
	if (a_RightDirectionPressed) { return iP2BlockUnit > iP1LevelUnit; }
	return iP2BlockUnit < iP1BlockUnit;
}

bool APlayerSpace::CheckForKillPosition(ABaseCharacter* a_Player)
{
	if (!a_Player) { return false;}
	if (a_Player->GetActorLocation().Z < fBottomKillPosition) { return true; }
	return false;
}

void APlayerSpace::ResetPlacement(ABaseCharacter* a_Player, FVector spawn)
{
	if (a_Player->GetPlayerNumber() == 1)
	{
		if (spawn == aa_pPlayer1Spawn->GetActorLocation())
		{
			iP1BlockUnit = iP1StartX;
		} else { iP1BlockUnit = iP2StartX;}
	}
	else
	{
		if (spawn == aa_pPlayer1Spawn->GetActorLocation())
		{
			iP2BlockUnit = iP1StartX;
		} else { iP2BlockUnit = iP2StartX;}
	}
	CheckWhoIsLeftRight();
}


bool APlayerSpace::IsHitDirectionLeft(ABaseCharacter* a_CharacterWhoCalled) const
{
	if (a_CharacterWhoCalled == nullptr) { return false; }
	if (a_CharacterWhoCalled == PlayerOne) {
		return iP1BlockUnit > iP2BlockUnit;
	}
	return iP2BlockUnit > iP1BlockUnit; 
}

void APlayerSpace::PlayCharacterAnimation(ABaseCharacter* a_character, EAnimationType animType) const
{
	if (a_character)
	{
		const ECharacterType Type = a_character->GetCharacterType();
		switch (animType)
		{
			case Attack:
				if (Type == DarkInvader) { a_character->GetSkeletalMeshDark()->PlayAnimation(a_AttackDark, false); }
				if (Type == YogaMaster) { a_character->GetSkeletalMeshYoga()->PlayAnimation(a_AttackYoga, false); }
				break;
			case Block:
				if (Type == DarkInvader) { a_character->GetSkeletalMeshDark()->PlayAnimation(a_BlockDark, false); }
				if (Type == YogaMaster) { a_character->GetSkeletalMeshYoga()->PlayAnimation(a_BlockYoga, false); }
				break;
			case Jump:
				if (Type == DarkInvader) { a_character->GetSkeletalMeshDark()->PlayAnimation(a_JumpDark, false); }
				if (Type == YogaMaster) { a_character->GetSkeletalMeshYoga()->PlayAnimation(a_JumpYoga, false); }
				break;
			case MoveForward:
				if (Type == DarkInvader) { a_character->GetSkeletalMeshDark()->PlayAnimation(a_MoveForwardDark, false); }
				if (Type == YogaMaster) { a_character->GetSkeletalMeshYoga()->PlayAnimation(a_MoveForwardYoga, false); }
				break;
			case MoveBackward:
				if (Type == DarkInvader) { a_character->GetSkeletalMeshDark()->PlayAnimation(a_MoveBackwardDark, false); }
				if (Type == YogaMaster) { a_character->GetSkeletalMeshYoga()->PlayAnimation(a_MoveBackwardYoga, false); }
				break;
			case JumpForward:
				if (Type == DarkInvader) { a_character->GetSkeletalMeshDark()->PlayAnimation(a_JumpForwardDark, false); }
				if (Type == YogaMaster) { a_character->GetSkeletalMeshYoga()->PlayAnimation(a_JumpForwardYoga, false); }
				break;
			case JumpBackWard:
				if (Type == DarkInvader) { a_character->GetSkeletalMeshDark()->PlayAnimation(a_JumpBackwardDark, false); }
				if (Type == YogaMaster) { a_character->GetSkeletalMeshYoga()->PlayAnimation(a_JumpBackwardYoga, false); }
				break;
			case SelfStab:
				if (Type == DarkInvader) { a_character->GetSkeletalMeshDark()->PlayAnimation(a_SelfStabDark, false); }
				if (Type == YogaMaster) { a_character->GetSkeletalMeshYoga()->PlayAnimation(a_SelfStabYoga, false); }
				break;
			case EAnimationType::TakeDamage:
				if (Type == DarkInvader) { a_character->GetSkeletalMeshDark()->PlayAnimation(a_TakeDamageDark, false); }
				if (Type == YogaMaster) { a_character->GetSkeletalMeshYoga()->PlayAnimation(a_TakeDamageYoga, false); }
				break;
			case Idle:
				if (Type == DarkInvader) { a_character->GetSkeletalMeshDark()->PlayAnimation(a_IdleDark, false); }
				if (Type == YogaMaster) { a_character->GetSkeletalMeshYoga()->PlayAnimation(a_IdleYoga, false); }
				break;
			default: 
				break;
		}
	}
}

bool APlayerSpace::CheckPlayerVerticalLayer()
{
	return iP1LevelUnit == iP2LevelUnit;
}

// Called every frame
void APlayerSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int APlayerSpace::AddPlayerEntity(ABaseCharacter* a_pBaseCharacter)
{
	if (!PlayerOne)
	{
		PlayerOne = a_pBaseCharacter;
		return 1;
	}
	PlayerTwo = a_pBaseCharacter;
	return 2;
}

bool APlayerSpace::CheckMovePlayerHorizontal(ABaseCharacter* a_pPlayer, bool a_bMoveRight, int a_iMovement)
{
	if (PlayerOne && PlayerTwo && a_pPlayer != nullptr)
	{
		if (PlayerOne == a_pPlayer)
		{
			if (a_bMoveRight)
			{
				// Player 1 is on the left and moves right
				return this->CheckMovement(iP1BlockUnit, iP2BlockUnit, a_iMovement);
			} 
			// Player 1 is on the right and moves left
			return this->CheckMovement(iP1BlockUnit, iP2BlockUnit, -a_iMovement);
		}
		if (a_bMoveRight)
		{
			// Player 2 is on the left and moves right
			return CheckMovement(iP2BlockUnit, iP1BlockUnit, a_iMovement);
		} 
		return this->CheckMovement(iP2BlockUnit, iP1BlockUnit, -a_iMovement);
		// Player 2 is on the right and moves left
	}
	return false;

}

void APlayerSpace::MovePlayerHorizontal(ABaseCharacter* a_pPlayer, bool a_bMoveRight, int a_iAmount)
{
	if (PlayerOne && PlayerTwo && a_pPlayer != nullptr)
	{
		if (PlayerOne == a_pPlayer)
		{
			if (a_bMoveRight) { iP1BlockUnit += a_iAmount; } else { iP1BlockUnit -= a_iAmount; }
		}
		else
		{
			if (a_bMoveRight) { iP2BlockUnit += a_iAmount; } else { iP2BlockUnit -= a_iAmount; }
		}
	}

	CheckWhoIsLeftRight();
	CheckBoundries(a_pPlayer);
}

void APlayerSpace::MovePlayerVertical(ABaseCharacter* a_pPlayer, bool a_bMoveUp)
{
	if (PlayerOne && PlayerTwo && a_pPlayer != nullptr)
	{
		if (PlayerOne == a_pPlayer)
		{
			iP1LevelUnit = a_bMoveUp ? iP1LevelUnit + 1 : iP1LevelUnit - 1;
		} else
		{
			iP2LevelUnit = a_bMoveUp ? iP2LevelUnit + 1 : iP2LevelUnit - 1;
		}
	}
}

bool APlayerSpace::CheckOtherPlayerSameLayer(ABaseCharacter* a_pPlayer, bool a_bRightDirection, int a_iAmount)
{
	return true;
}

void APlayerSpace::CheckWhoIsLeftRight()
{
	if (bPlayerOneLeft)
	{
		if (iP1BlockUnit > iP2BlockUnit) {
			bPlayerOneLeft = false;
			bSwitched = true;
		}
	} else
	{
		if (iP2BlockUnit > iP1BlockUnit) {
			bPlayerOneLeft = true;
			bSwitched = true;
		}
	}

	//Set Player values

	if (bSwitched)
	{
		bSwitched = false;
		if (bPlayerOneLeft)
		{
			if (PlayerOne) { PlayerOne->SetDirection(false); }
			if (PlayerTwo) { PlayerTwo->SetDirection(true); }
		} else
		{
			if (PlayerOne) { PlayerOne->SetDirection(true); }
			if (PlayerTwo) { PlayerTwo->SetDirection(false); }
		}
	}
}

void APlayerSpace::CheckBoundries(ABaseCharacter* a_pPlayer)
{
	if (a_pPlayer)
	{
		if (a_pPlayer == PlayerOne)
		{
			if (iP1BlockUnit <= iBlockMinBoundry || iP1BlockUnit >= iBlockMaxBoundry) { a_pPlayer->SetFallingState();} 
		} else
		{
			if (iP2BlockUnit <= iBlockMinBoundry || iP2BlockUnit >= iBlockMaxBoundry) { a_pPlayer->SetFallingState();} 
		}
	}
}

bool APlayerSpace::CheckIfOccupied(ABaseCharacter* a_pPlayer, FVector spawn)
{
	if (a_pPlayer->GetPlayerNumber() == 1)
	{
		if (iP1BlockUnit == iP2BlockUnit)
		{
			if (spawn == aa_pPlayer1Spawn->GetActorLocation())
			{
				iP1BlockUnit = iP2StartX;
			} else { iP1BlockUnit = iP1StartX; }
			return true;
		}
		return false;
	}

	if (iP2BlockUnit == iP1BlockUnit)
	{
		if (spawn == aa_pPlayer1Spawn->GetActorLocation())
		{
			iP2BlockUnit = iP2StartX;
		} else { iP2BlockUnit = iP1StartX;}
		return true;
	}
	return false;
}

FVector APlayerSpace::GetOtherSpawn(FVector spawn) const
{
	if (spawn == aa_pPlayer1Spawn->GetActorLocation()) { return aa_pPlayer2Spawn->GetActorLocation();}
	return aa_pPlayer1Spawn->GetActorLocation();
}

int APlayerSpace::getP1Block()
{
	return iP1BlockUnit;
}

int APlayerSpace::getP2Block()
{
	return iP2BlockUnit;
}

