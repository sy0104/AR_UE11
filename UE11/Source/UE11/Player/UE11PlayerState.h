// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerState.h"
#include "UE11PlayerState.generated.h"

// PlayerState�� PlayerController�� Pawn�� ������ �� ĳ������ PlayerState�� ������ �����͸� �����ϰ� �ȴ�.

UCLASS()
class UE11_API AUE11PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AUE11PlayerState();

protected:
	// Transient: �ֹ߼����� ���� �Ǵ� �ε尡 �ȵȴ�.
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
