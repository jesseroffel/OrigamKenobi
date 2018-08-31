// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ORIGAMKENOBI_API ABaseCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APlayerController* ThisPlayerController = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetControllerIndex(int a_iPlayerIndex, AActor* a_pWorldCamera);

	void KeyUp();

	void KeyDown();

	void KeyLeft();
	void KeyRight();

	void Attack();
	void Special();
	
};
