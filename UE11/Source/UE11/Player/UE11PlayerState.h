// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerState.h"
#include "UE11PlayerState.generated.h"

// PlayerState는 PlayerController가 Pawn에 빙의할 때 캐릭터의 PlayerState에 생성된 포인터를 전달하게 된다.

UCLASS()
class UE11_API AUE11PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AUE11PlayerState();

protected:
	// Transient: 휘발성으로 저장 또는 로드가 안된다.
	UPROPERTY(Transient)
	float mCameraZoomMin;

	UPROPERTY(Transient)
	float mCameraZoomMax;

public:
	float GetCameraZoomMin() const { return mCameraZoomMin; }
	float GetCameraZoomMax() const { return mCameraZoomMax; }

public:
	void InitPlayer();
};
