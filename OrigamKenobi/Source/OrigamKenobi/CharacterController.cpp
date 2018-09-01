// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterController.h"


ACharacterController::ACharacterController() : APlayerController()
{

}

void ACharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

