// Fill out your copyright notice in the Description page of Project Settings.

#include "OrigamKenobiGameModeBase.h"
#include "CharacterController.h"
#include "EmptyPawn.h"

AOrigamKenobiGameModeBase::AOrigamKenobiGameModeBase() : AGameModeBase()
{
	//tell your custom game mode to use your custom player controller
	PlayerControllerClass = ACharacterController::StaticClass();

	//you can set whatever (if any) other default framework classes
	//you wish for this game mode as well
	DefaultPawnClass = AEmptyPawn::StaticClass();
}
