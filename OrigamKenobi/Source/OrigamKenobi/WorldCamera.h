// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
		TArray<AActor*> aa_CharacterActors;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
