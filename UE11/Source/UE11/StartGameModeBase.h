// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint\UserWidget.h>

#include "GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "StartGameModeBase.generated.h"


UCLASS()
class UE11_API AStartGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AStartGameModeBase();

private:
	TSubclassOf<UUserWidget>	mStartHUDClass;
	UUserWidget*				mStartHUD;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
