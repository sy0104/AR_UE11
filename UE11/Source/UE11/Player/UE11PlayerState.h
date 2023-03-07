// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerState.h"
#include "UE11PlayerState.generated.h"

/*
PlayerState�� PlayerController�� Pawn�� �����Ҷ� ĳ������ PlayetState��
������ �����͸� �����س��� �ȴ�.
*/
UCLASS()
class UE11_API AUE11PlayerState : public APlayerState
{
	GENERATED_BODY()

	friend class APlayerCharacter;
	friend class AKnightCharacter;
	
public:
	AUE11PlayerState();

protected:
	// Transient : �ֹ߼����� ���� �Ǵ� �ε尡 �ȵȴ�.
	UPROPERTY(Transient)
	float	mCameraZoomMin;

	UPROPERTY(Transient)
	float	mCameraZoomMax;

	UPROPERTY(Transient)	
	FPlayerInfo	mPlayerInfo;

public:
	const FPlayerInfo& GetInfo()	const
	{
		return mPlayerInfo;
	}

	float GetCameraZoomMin()	const
	{
		return mCameraZoomMin;
	}

	float GetCameraZoomMax()	const
	{
		return mCameraZoomMax;
	}

public:
	void SetAttackDistance(float Dist)
	{
		mPlayerInfo.AttackDistance = Dist;
	}



public:
	void InitPlayer();
};
