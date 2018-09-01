// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCharacter.h"
#include "PlayerSpace.generated.h"

UCLASS()
class ORIGAMKENOBI_API APlayerSpace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerSpace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	int i_P1_BlockUnit = 0;	// X Axis
	int i_P1_LevelUnit = 0;	// Y Axis
	int i_P2_BlockUnit = 7;	// X Axis
	int i_P2_LevelUnit = 0;	// Y Axis

	ABaseCharacter* PlayerOne = nullptr;
	ABaseCharacter* PlayerTwo = nullptr;

	bool CheckMovement(int left, int right, int amount);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int AddPlayerEntity(ABaseCharacter* a_pBaseCharacter = nullptr);

	bool CheckMovePlayerHorizontal(ABaseCharacter* a_pPlayer = nullptr, bool a_bMoveRight = true, int a_iMovement = 1);

	void MovePlayerHorizontal(ABaseCharacter* a_pPlayer, bool a_bMoveRight, int a_iAmount);

	int getP1Block();
	int getP2Block();
	
};
