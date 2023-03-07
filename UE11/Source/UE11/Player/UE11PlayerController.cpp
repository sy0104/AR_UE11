// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11PlayerController.h"

AUE11PlayerController::AUE11PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	//bFindCameraComponentWhenViewTarget = true;
}

void AUE11PlayerController::BeginPlay()
{
	// Super :: 자기자신의 부모클래스를 의미한다.
	Super::BeginPlay();
}

void AUE11PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUE11PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAxis<>();
}

void AUE11PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// 플레이어 컨트롤러가 Pawn에 빙의될때 호출된다.
void AUE11PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

// 플레이어 컨트롤러가 빙의에서 해제될때 호출된다.
void AUE11PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}
