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

// InventoryItem �� �����ǰ� �� ��, ����Ʈ�� �Էµ� �����ͷκ��� InventoryItem �� �ʱ�ȭ�ȴ�.
void UInventoryItemBase::InitFromData(UObject* Data)
{
	UItemDataBase* pData = Cast<UItemDataBase>(Data);

	if (IsValid(pData))
	{
		// �Էµ� �����Ϳ� ����ִ� ������ InventoryItem �� �ʱ�ȭ �ȴ�.
		const FString& IconPath = pData->GetIconPath();
		const FString& ItemName = pData->GetDescription();
		int Count = pData->GetItemCount();

		// �����Ϳ� ����ִ� IconPath �� �̿��ؼ� �ش� �ؽ�ó�� �ε� ��, Image ������ �ش� �ؽ�ó�� �������ش�.
		UTexture2D* Tex2D = LoadObject<UTexture2D>(nullptr, *IconPath);

		if (IsValid(Tex2D))
			mIconImage->SetBrushFromTexture(Tex2D);

		// �����Ϳ� ����ִ� ������ �̸��� ItemName�� ����� �� �ؽ�Ʈ ��� ������ ����
		mItemNameText->SetText(FText::FromString(ItemName));

		// �����Ϳ� ����ִ� ������ ������ ���ڿ��� �ٲپ, ItemCount �ؽ�Ʈ��� ������ ����
		mCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
	}
}
