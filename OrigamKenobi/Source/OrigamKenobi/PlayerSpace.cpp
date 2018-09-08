// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSpace.h"
#include "Engine/Engine.h"


// Sets default values
APlayerSpace::APlayerSpace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerSpace::BeginPlay()
{
	Super::BeginPlay();
	bPlayerOneLeft = iP1BlockUnit < iP2BlockUnit ? true : false;
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
}

bool APlayerSpace::CheckMovement(int left, int right, int amount)
{
	if (left < right)
	{
		if ((left += amount) == right)
		{
			return false;
		}
		return true;
	}
	else
	{
		if ((left += amount) == right)
		{
			return false;
		}
		return true;
	}
}

bool APlayerSpace::CheckPlayerVerticalLayer()
{
	if (iP1LevelUnit == iP2LevelUnit) { return true; }
	return false;
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
			} else
			{
				// Player 1 is on the right and moves left
				return this->CheckMovement(iP1BlockUnit, iP2BlockUnit, -a_iMovement);
			}
		}
		if (a_bMoveRight)
		{
			// Player 2 is on the left and moves right
			return CheckMovement(iP2BlockUnit, iP1BlockUnit, a_iMovement);
		} else
		{
			return this->CheckMovement(iP2BlockUnit, iP1BlockUnit, -a_iMovement);
		}
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

int APlayerSpace::getP1Block()
{
	return iP1BlockUnit;
}

int APlayerSpace::getP2Block()
{
	return iP2BlockUnit;
}

