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
	
}

bool APlayerSpace::CheckMovement(int left, int right, int amount)
{
	if (left < right)
	{
		if ((left += amount) < right)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if ((left += amount) > right)
		{
			return true;
		}
		return false;
	}
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

bool APlayerSpace::CheckMovePlayerHorizontal(ABaseCharacter* a_pPlayer, bool a_bMoveRight)
{
	if (PlayerOne && PlayerTwo && a_pPlayer != nullptr)
	{
		if (PlayerOne == a_pPlayer)
		{
			if (a_bMoveRight)
			{
				// Player 1 is on the left and moves right
				return this->CheckMovement(i_P1_BlockUnit, i_P2_BlockUnit, 1);
			} else
			{
				// Player 1 is on the right and moves left
				return this->CheckMovement(i_P1_BlockUnit, i_P2_BlockUnit, -1);
			}
		}
		if (a_bMoveRight)
		{
			// Player 2 is on the left and moves right
			return CheckMovement(i_P2_BlockUnit, i_P1_BlockUnit, 1);
		} else
		{
			return this->CheckMovement(i_P2_BlockUnit, i_P1_BlockUnit, -1);
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
			if (a_bMoveRight) { i_P1_BlockUnit += a_iAmount; } else { i_P1_BlockUnit -= a_iAmount; }
		}
		else
		{
			if (a_bMoveRight) { i_P2_BlockUnit += a_iAmount; } else { i_P2_BlockUnit -= a_iAmount; }
		}
	}
}

int APlayerSpace::getP1Block()
{
	return i_P1_BlockUnit;
}

int APlayerSpace::getP2Block()
{
	return i_P2_BlockUnit;
}

