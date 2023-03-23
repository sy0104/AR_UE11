// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "UMG/SelectWidget.h"
#include "GameFramework/GameModeBase.h"
#include "SelectGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API ASelectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASelectGameMode();

private:
	TSubclassOf<USelectWidget>	mSelectHUDClass;
	USelectWidget* mSelectHUD;

public:
	USelectWidget* GetSelectWidget()
	{
		return mSelectHUD;
	}

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
