// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\ProgressBar.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UPlayerInfoBase : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UProgressBar*		mHPBar;
	UProgressBar*		mMPBar;

public:
	void SetHP(float Ratio);

public:
	virtual void NativeConstruct() override;										// BeginPlay() 역할
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;		// Tick() 역할
};
