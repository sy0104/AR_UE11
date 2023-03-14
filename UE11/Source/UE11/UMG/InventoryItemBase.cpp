// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemBase.h"
#include "ItemDataBase.h"

void UInventoryItemBase::NativeConstruct()
{
	Super::NativeConstruct();

	mIconImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("Icon"))));
	mItemNameText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ItemName"))));
	mCountText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("Count"))));
}

void UInventoryItemBase::NativeTick(const FGeometry& _geo, float _DT)
{
	Super::NativeTick(_geo, _DT);
}

// InventoryItem 이 생성되고 난 후, 리스트에 입력된 데이터로부터 InventoryItem 이 초기화된다.
void UInventoryItemBase::InitFromData(UObject* Data)
{
	UItemDataBase* pData = Cast<UItemDataBase>(Data);

	if (IsValid(pData))
	{
		// 입력된 데이터에 들어있는 값으로 InventoryItem 이 초기화 된다.
		const FString& IconPath = pData->GetIconPath();
		const FString& ItemName = pData->GetDescription();
		int Count = pData->GetItemCount();

		// 데이터에 들어있던 IconPath 를 이용해서 해당 텍스처를 로딩 후, Image 위젯에 해당 텍스처를 설정해준다.
		UTexture2D* Tex2D = LoadObject<UTexture2D>(nullptr, *IconPath);

		if (IsValid(Tex2D))
			mIconImage->SetBrushFromTexture(Tex2D);

		// 데이터에 들어있던 아이템 이름을 ItemName을 출력해 줄 텍스트 블록 위젯에 전달
		mItemNameText->SetText(FText::FromString(ItemName));

		// 데이터에 들어있던 아이템 수량을 문자열로 바꾸어서, ItemCount 텍스트블록 위젯에 전달
		mCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
	}
}
