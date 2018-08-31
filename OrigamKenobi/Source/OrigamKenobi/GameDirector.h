// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCharacter.h"
#include "WorldCamera.h"
#include "GameDirector.generated.h"

UCLASS()
class ORIGAMKENOBI_API AGameDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreatePlayers();

	int i_PlayerIndexCount = 0;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		TArray<ABaseCharacter*> aa_Characters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSetup")
		int i_PlayerCount = 1;


};
