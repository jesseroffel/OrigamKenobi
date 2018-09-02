// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "BaseCharacter.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APlayerController* ThisPlayerController = nullptr;
	// Direction = LEFT == TRUE, RIGHT == FALSE
	bool b_MirroredDirection = false;

	APlayerSpace* pPlayerSpace = nullptr;

	int iPlayerNumber = 0;

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
	void ResetToBottom();

	float HorizontalMovement = 50.0f;
	float VerticalMovement = 62.5f;
	float CharacterRotation = 45.0f;

public:

	UStaticMeshComponent* SphereVisual = nullptr;
	UStaticMeshComponent* SM_Yoga = nullptr;
	UStaticMeshComponent* SM_DarkInvader = nullptr;

	UBoxComponent* CharacterHitBox = nullptr;
	UBoxComponent* SwordHitBox = nullptr;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetControllerIndex(int a_iPlayerIndex, AActor* a_pWorldCamera);

	void SetPlayerMovementController(APlayerSpace* a_pPlayerSpace);

	void BindPlayer(int a_iIndex);

	void SignMyselfUpForMovement();

	void KeyUp();
	void KeyDown();
	void KeyLeft();
	void KeyRight();

	void Attack();
	void Special();


	void SetModelVisibleYoga();
	void SetModelVisibleVader();

	void SetDirection(bool a_bState);

	void OverLapFunction();

	bool GetAttackable();

	void MoveLeft();
	void MoveRight();

	void CheckPlayerMove();
	void CheckPlayerJump();

	void SideAttack();
	void BlockAttack();

};
