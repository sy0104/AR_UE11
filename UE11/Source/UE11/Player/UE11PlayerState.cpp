// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11PlayerState.h"
#include "../UE11SaveGame.h"
#include "../UE11GameModeBase.h"

AUE11PlayerState::AUE11PlayerState()
{
	mCameraZoomMin = 100.f;
	mCameraZoomMax = 500.f;
	mPlayerInfo.AttackDistance = 200.f;
}

void AUE11PlayerState::InitPlayer()
{
	mCameraZoomMin = 100.f;
	mCameraZoomMax = 500.f;
	mPlayerInfo.AttackDistance = 200.f;
}

void AUE11PlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AUE11GameModeBase* GameMode = GetWorld()->GetAuthGameMode<AUE11GameModeBase>();

	GameMode->GetSaveGame()->mPlayerInfo = mPlayerInfo;
	GameMode->GetSaveGame()->mCameraZoomMin = mCameraZoomMin;
	GameMode->GetSaveGame()->mCameraZoomMax = mCameraZoomMax;
}
