// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11PlayerController.h"

AUE11PlayerController::AUE11PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	//bFindCameraComponentWhenViewTarget = true;
}

void AUE11PlayerController::BeginPlay()
{
	// Super :: �ڱ��ڽ��� �θ�Ŭ������ �ǹ��Ѵ�.
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

// �÷��̾� ��Ʈ�ѷ��� Pawn�� ���ǵɶ� ȣ��ȴ�.
void AUE11PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

// �÷��̾� ��Ʈ�ѷ��� ���ǿ��� �����ɶ� ȣ��ȴ�.
void AUE11PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}
