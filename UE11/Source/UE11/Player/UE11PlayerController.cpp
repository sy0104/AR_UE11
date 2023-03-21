// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11PlayerController.h"
#include "../Particle/Decal.h"
#include "../CharacterBase.h"

AUE11PlayerController::AUE11PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	//bFindCameraComponentWhenViewTarget = true;
	bShowMouseCursor = true;
}

void AUE11PlayerController::BeginPlay()
{
	// Super :: �ڱ��ڽ��� �θ�Ŭ������ �ǹ��Ѵ�.
	Super::BeginPlay();

	// FInputModeUIOnly
	// FInputModeGameOnly
	// FInputModeGameAndUI

	FInputModeGameAndUI	Mode;
	SetInputMode(Mode);

	mMousePick = GetWorld()->SpawnActor<ADecal>(FVector::ZeroVector, FRotator::ZeroRotator);

	mMousePick->SetDecalMaterial(TEXT("MaterialInstanceConstant'/Game/Materials/MTMagicCircle_Inst.MTMagicCircle_Inst'"));
	mMousePick->SetSpawnType(EDecalSpawnType::Floor);

	// ���콺 ���� �ִ� ���͸� �����ϴ� ��� On
	bEnableMouseOverEvents = true;
}

void AUE11PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult	result;
	bool Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel8, false, result);

	if (Hit)
	{
		if (mPickActor.IsValid() && mPickActor.Get() != result.GetActor())
		{
			ACharacterBase* CharacterBase = Cast<ACharacterBase>(mPickActor.Get());

			if (IsValid(CharacterBase))
				CharacterBase->OnOutLine(false);
		}


		if (mPickActor.Get() != result.GetActor())
		{
			mPickActor = result.GetActor();

			// ��ü�� Player�� Monster�� ��� �ƿ������� ó���Ѵ�.
			ACharacterBase* CharacterBase = Cast<ACharacterBase>(result.GetActor());

			if (IsValid(CharacterBase))
			{
				mMousePick->SetDecalVisibility(false);
				CharacterBase->OnOutLine(true);
			}

			else
			{
				mMousePick->SetDecalVisibility(true);
				mMousePick->SetActorLocation(result.ImpactPoint);
			}
		}
	}

	else if (mPickActor.IsValid())
	{
		ACharacterBase* CharacterBase = Cast<ACharacterBase>(mPickActor.Get());

		if (IsValid(CharacterBase))
			CharacterBase->OnOutLine(false);

		mPickActor = nullptr;
		mMousePick->SetDecalVisibility(false);
	}

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

void AUE11PlayerController::SpawnMousePick()
{
	if (mPickActor.IsValid())
	{
		ACharacterBase* CharacterBase = Cast<ACharacterBase>(mPickActor.Get());

		if (IsValid(CharacterBase))
			return;
	}

	else
		return;

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADecal* Decal =
		GetWorld()->SpawnActor<ADecal>(
			mMousePick->GetActorLocation(), FRotator(0.f, mMousePick->GetActorRotation().Yaw, 0.f), SpawnParam);

	Decal->SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	Decal->SetDecalMaterial(TEXT("MaterialInstanceConstant'/Game/Materials/MTMagicCircle_Inst.MTMagicCircle_Inst'"));
	Decal->SetSpawnType(EDecalSpawnType::Floor);
	Decal->SetLifeSpan(5.f);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, mMousePick->GetActorLocation());
}
