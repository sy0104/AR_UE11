// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDBase.h"

void UMainHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_PlayerInfo = Cast<UPlayerInfoBase>(GetWidgetFromName(FName(TEXT("PlayerInfo"))));
	m_Inventory = Cast<UInventoryBase>(GetWidgetFromName(FName(TEXT("Inventory"))));
}

void UMainHUDBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);


}


void UMainHUDBase::SetHP(float _Ratio)
{
	m_PlayerInfo->SetHP(_Ratio);
}
