// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoBase.h"


void UPlayerInfoBase::NativeConstruct()
{
	Super::NativeConstruct();

	mHPBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("HPBar"))));
	mMPBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("MPBar"))));
}

void UPlayerInfoBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void UPlayerInfoBase::SetHP(float Ratio)
{
	mHPBar->SetPercent(Ratio);
}