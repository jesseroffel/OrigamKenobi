// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "BaseCharacter.generated.h"

UENUM()
enum ECharacterType
{
	UNSET				UMETA(DisplayName = "UNSET"),
	YogaMaster			UMETA(DisplayName = "YogaMaster"),
	DarkInvader			UMETA(DisplayName = "DarkInvader"),
};

class APlayerSpace;

UCLASS()
class ORIGAMKENOBI_API ABaseCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseCharacter();


	UFUNCTION()
		void OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void OnSpecialBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	ECharacterType GetCharacterType() const { return CharacterType; }
	USkeletalMeshComponent* GetSkeletalMeshDark() const { return SK_DarkInvader; }
	USkeletalMeshComponent* GetSkeletalMeshYoga() const { return SK_YogaMaster; }
	void SetFallingState();
	int GetPlayerNumber() const { return iPlayerNumber; }
	void Respawn();
	int GetHealth() { return iHealth; }
	int GetSpecial() { return iSpecial; }
	FString GetCharacterName() const { return CharacterName; }
protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	APlayerController* ThisPlayerController = nullptr;
	// Direction = LEFT == TRUE, RIGHT == FALSE
	bool b_MirroredDirection = false;
	bool bSelfHit = false;

	APlayerSpace* pPlayerSpace = nullptr;
	ECharacterType CharacterType = UNSET;
	int iPlayerNumber = 0;

	int iLives = 3;
	int iHealth = 100;
	int iSpecial = 100;
	const int iSpecialMax = 100;
	float fSpecialAddWait = 1.0f;
	bool bSpecialPressed = false;
	bool bSpecialActivated = false;
	float fSpecialPunishTimer = 0.0f;
	float fSpecialTimeToPunish = 5.0f;


	bool bMovementLocked = false;
	bool bMoving = false;
	bool bCheckMoveTimer = false;
	float fBeforeMovingTimer = 0.0f;

	bool bHorizontalLocked = false;
	bool bJumping = false;
	bool bVerticalLocked = false;
	bool bVerticalReset = false;
	bool bCheckJumpTimer = false;
	bool bJumpMoving = false;
	bool bJumpFinishing = false;
	bool bJumpingPressed = false;
	bool bJumpDirection = false;
	FVector vGoingLocation;
	FVector vCharacterLocation;

	//ATTACKING 
	bool bAttackable = true;
	bool bAttacking = false;
	bool bCheckAttackTimer = false;
	float fAttackingTimer = 0.0f;

	bool bBlocking = false;
	float fBlockTimer = 0.0f;
	bool bCheckBlockTimer = 0.0f;

	bool bActionButtonPressed = false;
	bool bDirectionPressed = false;
	bool bRightDirectionPressed = false;
	bool bCombinationTriggered = false;

	float fJumpTimer = 0.0f;

	float HorizontalMovement = 50.0f;
	float VerticalMovement = 62.5f;
	float CharacterRotation = 45.0f;

	bool bCheckOverlapAttack = false;
	bool bSuccessfulHit = false;
	float fCheckOverlapAttackTimer = 0.0f;

	bool bStunned = false;
	float fStunTimer = 0.0f;

	//Pit 
	bool bFalling = false;
	bool bMarkedForFalling = false;
	float fMarkedForFallTimer = 0.0f;
	bool bBottomReached = false;

public:

	UStaticMeshComponent* SphereVisual = nullptr;
	USkeletalMeshComponent* SK_YogaMaster = nullptr;
	USkeletalMeshComponent* SK_DarkInvader = nullptr;

	UBoxComponent* CharacterHitBox = nullptr;
	UBoxComponent* SwordHitBox = nullptr;
	UBoxComponent* SpecialHitBox = nullptr;

	FString CharacterName = "NAME_ME_PLZ";

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Input functions
	void KeyUp();
	void KeyLeft();
	void KeyRight();
	void Attack();
	void Special();

	void SetControllerIndex(int a_iPlayerIndex, AActor* a_pWorldCamera);
	void SetPlayerMovementController(APlayerSpace* a_pPlayerSpace);
	void BindPlayer(int a_iIndex);
	void SignMyselfUpForMovement();

	void SetModelVisibleYoga();
	void SetModelVisibleVader();

	void ResetToBottom();

	void SetDirection(bool a_bState);

	bool GetAttackable() const;

	void MoveLeft();
	void MoveRight();

	void CheckPlayerMove();
	void CheckPlayerJump();
	void CheckPlayerAttacking(float const fWorldTimer);
	void CheckPlayerBlocking(float const fWorldTimer);

	void SideAttack();
	void BlockAttack();
	void AttackHitMe(bool a_bLeftDirection = false);

	void RemoveStun();

	void AddSpecialPoints(int a_iAmount);

	void ActivateSpecial();
	void DisableSpecial();
};
