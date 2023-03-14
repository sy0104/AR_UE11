// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDBase.h"


void UMainHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	// GetWidgetFromName(): 자기가 소유한 위젯 중 이름을 통해 찾아서 리턴해준다.
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
