// Fill out your copyright notice in the Description page of Project Settings.

#include "GameDirector.h"
#include "BaseCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


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
	this->CreatePlayers();
}

void AGameDirector::CreatePlayers()
{
	FVector location(50.f, 50.0f, 0);
	if (i_PlayerCount > 0 && i_PlayerCount < 5)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldCamera::StaticClass(), FoundActors);
		ABaseCharacter* basechar = (ABaseCharacter*)GetWorld()->SpawnActor<ABaseCharacter>(ABaseCharacter::StaticClass(), location, FRotator::ZeroRotator);
		if (FoundActors.Num() > 0)
		{
			for (int i = 0; i < i_PlayerCount; i++)
			{
				ABaseCharacter* basechar = (ABaseCharacter*) GetWorld()->SpawnActor<ABaseCharacter>(ABaseCharacter::StaticClass(), location, FRotator::ZeroRotator);
				if (basechar)
				{
					UE_LOG(LogTemp, Warning, TEXT("Actor spawned successfully"));
				}
				
				//basechar->SetControllerIndex(i_PlayerIndexCount, FoundActors[0]);
				//aa_Characters.Add(basechar);

				//i_PlayerIndexCount++;
			}

		}
	} 
	
}

// Called every frame
void AGameDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

