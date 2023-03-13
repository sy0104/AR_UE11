// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint\UserWidget.h>

#include "PlayerInfoBase.h"

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UMainHUDBase : public UUserWidget
{
	GENERATED_BODY()

private:
	UPlayerInfoBase*	mPlayerInfo;	// MainHUD에서 PlayerInfo에 빠르기 접근하기 위한 포인터 변수
	
public:
	// Ratio: 0.f ~ 1.f
	void SetHP(float Ratio);

public:
	virtual void NativeConstruct() override;										// BeginPlay() 역할
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;		// Tick() 역할
};
