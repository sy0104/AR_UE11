// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDBase.h"


void UMainHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	// GetWidgetFromName(): �ڱⰡ ������ ���� �� �̸��� ���� ã�Ƽ� �������ش�.
	mPlayerInfo = Cast<UPlayerInfoBase>(GetWidgetFromName(FName(TEXT("PlayerInfo"))));
	mInventory = Cast<UInventoryBase>(GetWidgetFromName(FName(TEXT("Inventory"))));
}

void UMainHUDBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);

}

void UMainHUDBase::SetHP(float Ratio)
{
	mPlayerInfo->SetHP(Ratio);
}
