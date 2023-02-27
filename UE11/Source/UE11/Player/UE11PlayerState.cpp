// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11PlayerState.h"

AUE11PlayerState::AUE11PlayerState()
{
	mCameraZoomMin = 100.f;
	mCameraZoomMax = 500.f;
	
	// 전방으로 2m 공격할 수 있도록 설정
	mPlayerInfo.AttackDistance = 200.f;
}

void AUE11PlayerState::InitPlayer()
{
	mCameraZoomMin = 100.f;
	mCameraZoomMax = 500.f;

	mPlayerInfo.AttackDistance = 200.f;
}
