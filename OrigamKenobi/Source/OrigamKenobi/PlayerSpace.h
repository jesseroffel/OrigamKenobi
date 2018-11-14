// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCharacter.h"
#include "Animation/AnimSequence.h"
#include "PlayerSpace.generated.h"


UENUM()
enum EAnimationType
{
	NoAnim			UMETA(DisplayName = "NoAnim"),
	Attack			UMETA(DisplayName = "Attack"),
	Block			UMETA(DisplayName = "Block"),
	Jump			UMETA(DisplayName = "Jump"),
	MoveForward     UMETA(DisplayName = "MoveForward")
};


UCLASS()
class ORIGAMKENOBI_API APlayerSpace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerSpace();

	bool IsHitDirectionLeft(ABaseCharacter* a_CharacterWhoCalled = nullptr) const;
	void PlayCharacterAnimation(ABaseCharacter* a_character = nullptr, EAnimationType animType = NoAnim) const;
	bool HitMySelf(ABaseCharacter* a_Player = nullptr, bool a_RightDirectionPressed = false);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 0=0 1=50 2=100 3=150 4=200 5=250 6=300 7=350 8=400 9=450 10=500 11=550 12=600 13=650 14=700 15=750
	// 16 steps on board, -1 && 16 = death	0-15 range
	int iP1BlockUnit = 5;	// X Axis
	int iP1LevelUnit = 0;	// Y Axis
	int iP2BlockUnit = 11;	// X Axis
	int iP2LevelUnit = 0;	// Y Axis

	bool bPlayerOneLeft = true;
	bool bSwitched = false;

	ABaseCharacter* PlayerOne = nullptr;
	ABaseCharacter* PlayerTwo = nullptr;

	bool CheckMovement(int left, int right, int amount);

	//Dark Invader animations
	UAnimationAsset* a_AttackDark = nullptr;
	UAnimationAsset* a_BlockDark = nullptr;
	UAnimationAsset* a_JumpDark = nullptr;
	UAnimationAsset* a_MoveForwardDark = nullptr; 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int AddPlayerEntity(ABaseCharacter* a_pBaseCharacter = nullptr);

	bool CheckMovePlayerHorizontal(ABaseCharacter* a_pPlayer = nullptr, bool a_bMoveRight = true, int a_iMovement = 1);

	void MovePlayerHorizontal(ABaseCharacter* a_pPlayer, bool a_bMoveRight, int a_iAmount);

	bool CheckPlayerVerticalLayer();

	void MovePlayerVertical(ABaseCharacter* a_pPlayer, bool a_bMoveUp);

	bool CheckOtherPlayerSameLayer(ABaseCharacter* a_pPlayer, bool a_bRightDirection, int a_iAmount);

	void CheckWhoIsLeftRight();

	int getP1Block();
	int getP2Block();
	
};
