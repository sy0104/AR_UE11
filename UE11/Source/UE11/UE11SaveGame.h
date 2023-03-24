// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "GameFramework/SaveGame.h"
#include "UE11SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UUE11SaveGame : public USaveGame
{
	GENERATED_BODY()

	friend class APlayerCharacter;
	friend class AUE11PlayerState;
	friend class AUE11GameModeBase;
	
private:
	UPROPERTY()
	FPlayerInfo	mPlayerInfo;

	UPROPERTY()
	float	mCameraZoomMin;

	UPROPERTY()
	float	mCameraZoomMax;

	UPROPERTY()
	FVector	mLocation;

	UPROPERTY()
	FVector	mScale;

	UPROPERTY()
	FRotator	mRotation;
};
