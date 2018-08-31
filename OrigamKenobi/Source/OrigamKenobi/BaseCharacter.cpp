// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	//Create and poisition a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("KeyUp", IE_Pressed, this, &ABaseCharacter::KeyUp);
	InputComponent->BindAction("KeyDown", IE_Pressed, this, &ABaseCharacter::KeyDown);
	InputComponent->BindAction("KeyLeft", IE_Pressed, this, &ABaseCharacter::KeyLeft);
	InputComponent->BindAction("KeyRight", IE_Pressed, this, &ABaseCharacter::KeyRight);

	InputComponent->BindAction("Attack", IE_Pressed, this, &ABaseCharacter::Attack);
	InputComponent->BindAction("Special", IE_Pressed, this, &ABaseCharacter::Special);
}

void ABaseCharacter::SetControllerIndex(int a_iPlayerIndex, AActor* a_pWorldCamera)
{
	ThisPlayerController = UGameplayStatics::GetPlayerController(this, a_iPlayerIndex);
	if (ThisPlayerController)
	{
		ThisPlayerController->SetViewTarget(a_pWorldCamera);
	}
}

void ABaseCharacter::KeyUp()
{
}

void ABaseCharacter::KeyDown()
{
}

void ABaseCharacter::KeyLeft()
{
}

void ABaseCharacter::KeyRight()
{
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Special()
{
}

