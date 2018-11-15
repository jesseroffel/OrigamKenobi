// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDirector.h"
#include "BaseCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "WorldCamera.h"


// Sets default values
AGameDirector::AGameDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameDirector::BeginPlay()
{
	Super::BeginPlay();
	if (aa_pPlayer1Spawn && aa_pPlayer2Spawn)
	{
		this->CreatePlayers();
	}
}

void AGameDirector::CreatePlayers()
{
	if (i_PlayerCount > 0 && i_PlayerCount < 5)
	{
		bool bNoCollisionFail = true;
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = bNoCollisionFail ? ESpawnActorCollisionHandlingMethod::AlwaysSpawn : ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldCamera::StaticClass(), FoundActors);
		AWorldCamera* camera = dynamic_cast<AWorldCamera*>(FoundActors[0]);
		if (FoundActors.Num() > 0 && FoundActors[0])
		{
			//Player 1
			ABaseCharacter* P1 = GetWorld()->SpawnActor<ABaseCharacter>(ABaseCharacter::StaticClass(), aa_pPlayer1Spawn->GetActorLocation(), FRotator::ZeroRotator, ActorSpawnParams);
			if (P1)
			{
				aa_Characters.Emplace(P1);
				P1->BindPlayer(0);
				P1->SetControllerIndex(0, FoundActors[0]);
				if (pPlayerSpace != nullptr)
				{
					P1->SetPlayerMovementController(pPlayerSpace);
					P1->SignMyselfUpForMovement();
				}
				//P1->SetModelVisibleVader();
				P1->SetModelVisibleYoga();
				camera->aa_CharacterActors.Emplace(P1);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("Actor could not be spawned!")); }
			//Player 2

			
			UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
			ABaseCharacter* P2 = GetWorld()->SpawnActor<ABaseCharacter>(ABaseCharacter::StaticClass(), aa_pPlayer2Spawn->GetActorLocation(), FRotator::ZeroRotator, ActorSpawnParams);
			if (P2)
			{
				aa_Characters.Emplace(P2);
				P2->BindPlayer(1);
				P2->SetControllerIndex(1, FoundActors[0]);
				if (pPlayerSpace != nullptr)
				{
					P2->SetPlayerMovementController(pPlayerSpace);
					P2->SignMyselfUpForMovement();
				}
				//P2->SetModelVisibleYoga();
				P2->SetModelVisibleVader();
				camera->aa_CharacterActors.Emplace(P2);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("Actor could not be spawned!")); }
		}
	} 
}

FVector AGameDirector::RespawnThisPlayer(ABaseCharacter* a_character)
{
	if (a_character && aa_Characters.Num() >= 2)
	{
		float spawnPoint1 = aa_pPlayer1Spawn->GetActorLocation().X;
		float spawnPoint2 = aa_pPlayer2Spawn->GetActorLocation().X;
		float P1 = aa_Characters[0]->GetActorLocation().X;
		float P2 = aa_Characters[1]->GetActorLocation().X;
		float Selected = 0.0f;
		// Player One
		if (a_character == aa_Characters[0])
		{
			if (P1 > spawnPoint1 && P1 > spawnPoint2 && P2 < spawnPoint1) { Selected = spawnPoint2; } else { Selected = spawnPoint1; }
			if (P1 < spawnPoint1 && P1 < spawnPoint2 && P2 > spawnPoint1) { Selected = spawnPoint1; } else { Selected = spawnPoint2; }
			if (Selected == spawnPoint1) { return aa_pPlayer1Spawn->GetActorLocation(); } 
			return aa_pPlayer2Spawn->GetActorLocation();
		}
		if (P2 > spawnPoint1 && P2 > spawnPoint2 && P1 < spawnPoint1) { Selected = spawnPoint2; } else { Selected = spawnPoint1; }
		if (P2 < spawnPoint1 && P2 < spawnPoint2 && P1 > spawnPoint1) { Selected = spawnPoint1; } else { Selected = spawnPoint2; }
		if (Selected == spawnPoint1) { return aa_pPlayer1Spawn->GetActorLocation(); } 
		return aa_pPlayer2Spawn->GetActorLocation();
	}
	return aa_pPlayer1Spawn->GetActorLocation();
}

// Called every frame
void AGameDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

