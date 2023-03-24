// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectPlayerController.h"


ASelectPlayerController::ASelectPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	//bFindCameraComponentWhenViewTarget = true;
	bShowMouseCursor = true;

	// ���콺 ���� �ִ� ���͸� �����ϴ� ��� ��
	bEnableMouseOverEvents = true;

	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableTouchOverEvents = true;
}

void ASelectPlayerController::BeginPlay()
{
	// Super :: �ڱ��ڽ��� �θ�Ŭ������ �ǹ��Ѵ�.
	Super::BeginPlay();

	// FInputModeUIOnly
	// FInputModeGameOnly
	// FInputModeGameAndUI

	FInputModeGameAndUI	Mode;
	SetInputMode(Mode);

}

void ASelectPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult	result;
	bool Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,
		false, result);

	if (Hit)
	{
	}
}

void ASelectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAxis<>();
}
