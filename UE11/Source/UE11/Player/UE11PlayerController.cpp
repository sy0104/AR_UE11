// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11PlayerController.h"
#include "../Particle/Decal.h"

AUE11PlayerController::AUE11PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	//bFindCameraComponentWhenViewTarget = true;
	bShowMouseCursor = true;
}

void AUE11PlayerController::BeginPlay()
{
	// Super :: 자기자신의 부모클래스를 의미한다.
	Super::BeginPlay();

	FInputModeGameAndUI Mode;
	SetInputMode(Mode);

	mMousePick = GetWorld()->SpawnActor<ADecal>(FVector::ZeroVector, FRotator::ZeroRotator);
	mMousePick->SetDecalMaterial(TEXT("MaterialInstanceConstant'/Game/Materials/MTMagicCircle_Inst.MTMagicCircle_Inst'"));
	mMousePick->SetSpawnType(EDecalSpawnType::Floor);
}

void AUE11PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult result;
	bool Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel8, false, result);

	if (Hit)
		mMousePick->SetActorLocation(result.ImpactPoint);

	mMousePick->AddActorWorldRotation(FRotator(0.f, 180.f * DeltaTime, 0.f));
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

void AUE11PlayerController::SpawnMousePick()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADecal* Decal = GetWorld()->SpawnActor<ADecal>(mMousePick->GetActorLocation(),
			FRotator(0.f, mMousePick->GetActorRotation().Yaw, 0.f), SpawnParam);

	Decal->SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	Decal->SetDecalMaterial(TEXT("MaterialInstanceConstant'/Game/Materials/MTMagicCircle_Inst.MTMagicCircle_Inst'"));
	Decal->SetSpawnType(EDecalSpawnType::Floor);
	Decal->SetLifeSpan(5.f);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, mMousePick->GetActorLocation());
}
