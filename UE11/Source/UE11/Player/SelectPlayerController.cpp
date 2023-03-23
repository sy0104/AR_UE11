// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectPlayerController.h"

ASelectPlayerController::ASelectPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;

	// 마우스 위에 있는 액터를 인지하는 기능 on
	bEnableMouseOverEvents = true;

	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableTouchOverEvents = true;
}

void ASelectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI Mode;
	SetInputMode(Mode);
}

void ASelectPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult result;
	bool Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, result);

	if (Hit)
	{

	}
}

void ASelectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
