// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\ProgressBar.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHPBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UMonsterHPBase : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UProgressBar*		mHPBar;
	float				mInitRatio;

public:
	void SetInitHP(float Ratio)
	{
		mInitRatio = Ratio;
	}

	// BeginPlay에서 호출하면 안됨
	void SetHP(float Ratio);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;

};
