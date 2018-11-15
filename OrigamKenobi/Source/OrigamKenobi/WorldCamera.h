// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "WorldCamera.generated.h"

UCLASS()
class ORIGAMKENOBI_API AWorldCamera : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AWorldCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharactersToFollow")
	TArray<ABaseCharacter*> aa_CharacterActors;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSettings")
	float DistanceXMin = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSettings")
	float DistanceXMax = 400.0f;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float fCharacterMiddlePoint = 0.0f;
	float fCharacterDistance = 0.0f;
	USpringArmComponent* pSpringArm = nullptr;
};
