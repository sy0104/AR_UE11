// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHPBase.h"

void UMonsterHPBase::NativeConstruct()
{
	Super::NativeConstruct();

	mHPBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("HPBar"))));
	mHPBar->SetPercent(mInitRatio);
}

void UMonsterHPBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void UMonsterHPBase::SetHP(float Ratio)
{
	if (IsValid(mHPBar))
	{
		mHPBar->SetPercent(Ratio);
	}
}
