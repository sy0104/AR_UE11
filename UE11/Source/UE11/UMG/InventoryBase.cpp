// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBase.h"
#include "ItemDataBase.h"

void UInventoryBase::NativeConstruct()
{
	Super::NativeConstruct();

	mListView = Cast<UListView>(GetWidgetFromName(FName(TEXT("ListView"))));
	mListView->SetVisibility(ESlateVisibility::Visible);

	// Item Ãß°¡
	UItemDataBase* NewData = NewObject<UItemDataBase>();
	
	NewData->SetIconPath(TEXT("Texture2D'/Game/Viking_RPG_UI_5_0/Buttons/Standart_buttons/Flat_Icon_18_a.Flat_Icon_18_a'"));
	NewData->SetDescription(TEXT("Mega Potion"));
	NewData->SetItemCount(5);

	mListView->AddItem(NewData);
}

void UInventoryBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}
