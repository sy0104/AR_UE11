// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11PlayerState.h"

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
