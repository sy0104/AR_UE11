// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBase.h"

#include "ItemDataBase.h"

void UInventoryBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_ListView = Cast<UListView>(GetWidgetFromName(FName(TEXT("ListView"))));
	

	UItemDataBase* pNewData = NewObject<UItemDataBase>();

	pNewData->SetIconPath(TEXT("Texture2D'/Game/Viking_RPG_UI_5_0/Buttons/Standart_buttons/Flat_Icon_18_a.Flat_Icon_18_a'"));
	pNewData->SetDescription(TEXT("Mega Potion"));
	pNewData->SetItemCount(5);

	m_ListView->AddItem(pNewData);
	
}

void UInventoryBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);


}